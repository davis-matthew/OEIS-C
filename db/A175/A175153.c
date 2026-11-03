/*
 * A175153  Numbers k such that 13*k-1, 13*k+1 are twin primes.
 * https://oeis.org/A175153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 199, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 988 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer k such that 13*k-1 and 13*k+1 are twin primes. */
/* The function searches over positive integers k, checks primality of 13*k±1,
   and caches found values to answer future calls efficiently. */

#include <stdbool.h>
#include <stddef.h>
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

static seq_t *cache = NULL;   /* Cached sequence values */
static size_t cache_cap = 0;  /* Current allocation capacity of cache */
static idx_t cache_len = 0;   /* Number of values already computed */

static seq_t last_k = 0;      /* Largest k found so far (in cache) */

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    /* Return cached values when already computed. */
    if (cache_len >= n)
        return cache[n - 1];

    /* Start searching from the next multiple of 6 after the last found k. */
    seq_t k = (cache_len == 0) ? (seq_t)6 : last_k + (seq_t)6;

    while (cache_len < n) {
        /* Compute 13 * k and check twin primes. */
        seq_t pk = (seq_t)13 * k;
        seq_t p1 = pk - (seq_t)1;   /* 13*k - 1 */
        seq_t p2 = pk + (seq_t)1;   /* 13*k + 1 */

        if (is_prime_big(p1) && is_prime_big(p2)) {
            /* Store the found k in the cache. */
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = k;
            last_k = k;
        }

        k += (seq_t)6; /* Next candidate k must be multiple of 6 */
    }

    return cache[n - 1];
}
