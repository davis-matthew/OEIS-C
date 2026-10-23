/*
 * A130418  Triangle (of n terms per row) where t(n,m) = the m-th positive
 *          integer which is coprime to n!.
 * https://oeis.org/A130418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   triangular_prefix
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the m‑th positive integer coprime to r! where
   r is the smallest integer with r*(r+1)/2 >= n and
   m = n - r*(r-1)/2 (the 1‑based position within that row). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/triangular_prefix.h"

/* G‑lobal cache of primes up to the largest limit requested. */
static uint32_t *g_primes   = NULL;
static size_t   g_prime_cnt = 0;
static idx_t    g_prime_max = 0;

/* Generate all primes ≤ limit and store them in the global array. */
static void generate_primes_up_to(idx_t limit)
{
    if (limit <= g_prime_max)               /* already enough */
        return;

    size_t sz = (size_t)limit + 1;
    bool *sieve = (bool*)malloc(sz * sizeof(bool));
    if (!sieve)
        return;                           /* allocation failure – give up */

    for (size_t i = 0; i < sz; ++i) sieve[i] = true;
    if (sz > 0) sieve[0] = false;
    if (sz > 1) sieve[1] = false;

    for (size_t p = 2; p * p <= limit; ++p) {
        if (sieve[p]) {
            for (size_t q = p * p; q <= limit; q += p)
                sieve[q] = false;
        }
    }

    /* Count primes. */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++cnt;

    uint32_t *new_primes = (uint32_t*)realloc(g_primes, cnt * sizeof(uint32_t));
    if (!new_primes) {
        free(sieve);
        return;
    }
    g_primes = new_primes;

    /* Copy primes. */
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (sieve[i]) g_primes[idx++] = (uint32_t)i;
    }

    g_prime_cnt = cnt;
    g_prime_max = limit;

    free(sieve);
}

/* Binary search for the smallest r such that r*(r+1)/2 >= n. */
static idx_t find_row(idx_t n)
{
    idx_t low  = 1;
    idx_t high = n;                 /* r cannot exceed n */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        idx_t tri = (mid * (mid + 1)) / 2;
        if (tri >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined below offset */

    /* Determine row r and position m within that row. */
    idx_t r = find_row(n);
    idx_t start = triangular_prefix(r);   /* first index of row r */
    idx_t m = n - start;                  /* 1‑based position */

    /* Ensure all primes up to r are available. */
    generate_primes_up_to(r);

    seq_t candidate = 1;
    idx_t count = 0;

    for (;;) {
        bool coprime = true;
        /* check divisibility by all primes <= r. */
        for (size_t i = 0; i < g_prime_cnt && g_primes[i] <= r; ++i) {
            seq_t p = (seq_t)g_primes[i];
            if ((candidate % p) == 0) {
                coprime = false;
                break;
            }
        }

        if (coprime) {
            ++count;
            if (count == m)
                return candidate;
        }
        ++candidate;
    }
}
