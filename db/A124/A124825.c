/*
 * A124825  Numbers k such that 10k + 7 and 7k + 10 are primes.
 * https://oeis.org/A124825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 403, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 964 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finding the n‑th integer k for which both 10k+7 and 7k+10 are prime. */
/* The sequence is built on demand and cached so that calls in any order
   are fast.  The routine uses is_prime_big which works for arbitrary‑size
   integers and is available through "../utils/is_prime_big.h". */
#include <stddef.h>
#include <stdbool.h>

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* static cache for discovered k values */
static seq_t *cache      = NULL;   /* array of found k's (as seq_t)          */
static size_t cache_len  = 0;      /* number of k's stored                   */
static size_t cache_cap  = 0;      /* current capacity of cache             */
static idx_t  next_k     = 1;      /* smallest candidate not yet tested      */

/* a(n) returns the n-th element (n>=1) of A124825 */
seq_t a(idx_t n)
{
    /* Ensure n is positive; behaviour for n<1 is undefined but will
       simply return 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Build cache until we have at least n elements. */
    while (cache_len < n) {
        /* Test current candidate k = next_k. */
        seq_t ck1 = 10 * (seq_t)next_k + (seq_t)7;
        seq_t ck2 =  7 * (seq_t)next_k + (seq_t)10;

        if (is_prime_big(ck1) && is_prime_big(ck2)) {
            /* Found a valid k; store it in the cache. */
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    /* Return the cached value.  The array is 0‑indexed while OEIS
       indices start at 1. */
    return cache[n - 1];
}
