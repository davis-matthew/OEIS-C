/*
 * A213632  Primes p such that primality of (p+p')/2+4 and of (p+p')/2-4
 *          differ, where p'=prevprime(p-1), the next smaller prime.
 * https://oeis.org/A213632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A213632: primes p such that primality of (p+p')/2+4 and of (p+p')/2-4 differ. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
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

/* Append a value to a dynamic array of seq_t. */
static void append_cache(seq_t **arr, size_t *len, size_t *cap, seq_t val) {
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 8 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, (newcap * sizeof(**arr)) / sizeof(seq_t));
        *cap = newcap;
    }
    (*arr)[*len] = val;
    ++(*len);
}

/* Cached state. */
static uint64_t *primes     = NULL;   /* ascending list of primes */
static size_t   primes_cnt  = 0;
static size_t   proc_idx    = 1;      /* next prime index to examine */

static seq_t *cache        = NULL;   /* sequence values found so far */
static size_t cache_len    = 0;
static size_t cache_cap    = 0;

/* One‑time initialization. */
static void init(void) {
    if (primes != NULL) return;
    const idx_t LIMIT = 20000000;            /* plenty for n up to ~10 000 */
    build_primes_upto(LIMIT, &primes, &primes_cnt);
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined; safe fallback */

    init();

    /* Generate terms until we have enough. */
    while (cache_len < n) {
        if (proc_idx >= primes_cnt) break;   /* unlikely for requested n */

        uint64_t p    = primes[proc_idx];
        uint64_t prev = primes[proc_idx - 1];

        seq_t m = ((seq_t)(p + prev)) / 2;     /* average of two consecutive primes */
        bool b1 = is_prime_big(m + 4);
        bool b2 = is_prime_big(m - 4);

        if (b1 != b2) {
            append_cache(&cache, &cache_len, &cache_cap, (seq_t)p);
        }
        ++proc_idx;
    }

    if (cache_len < n) return (seq_t)0;  /* graceful fallback if limit insufficient */
    return cache[n - 1];
}
