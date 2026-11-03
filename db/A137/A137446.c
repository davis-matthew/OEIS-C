/*
 * A137446  Positive numbers k such that k^2 +- (k-1) are primes.
 * https://oeis.org/A137446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer k such that k^2-(k-1) and
 *            k^2+(k-1) are both prime. The function builds the sequence on
 *            demand, caching the results for future calls
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 992 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* size_t */
#include <stdbool.h>     /* bool */

/* Helpers supplied by the framework */
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Cached sequence data */
static seq_t *a_cache      = NULL;   /* array of found k's          */
static size_t a_cache_len  = 0;      /* number of stored values     */
static size_t a_cache_cap  = 0;      /* allocated capacity          */
static seq_t  a_next_k     = 2;      /* next candidate to test      */

seq_t a(idx_t n)
{
    /* OEIS offset is 1 – for n==0 we return 0 as a safe default. */
    if (n == 0)
        return 0;

    /* Grow the cache until it contains at least n elements. */
    while (a_cache_len < (size_t)n)
    {
        seq_t k = a_next_k;

        /* Compute p1 = k^2 - (k-1) and p2 = k^2 + (k-1) using
           seq_t arithmetic (works for built‑in unsigned types as well
           as for the arbitrary‑precision class). */
        seq_t kk  = k * k;
        seq_t p1  = kk - k + 1;   /* k^2 - (k-1) */
        seq_t p2  = kk + k - 1;   /* k^2 + (k-1) */

        if (is_prime_big(p1) && is_prime_big(p2))
            append_candidate(&a_cache, &a_cache_len, &a_cache_cap, k);

        ++a_next_k;
    }

    return a_cache[n - 1];
}
