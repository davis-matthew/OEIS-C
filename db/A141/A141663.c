/*
 * A141663  Regular triangle T(n,m) = abs(prime(n) - m^2), 0 <= m <= n-1.
 * https://oeis.org/A141663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = |prime(k) - m²|, where k = smallest integer with k(k+1)/2
 *            ≥ n, and m = n - k(k-1)/2 - 1. (Row and column indices are
 *            0‑based.)
 * Range:     u8 n <= 188, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Validated: standard, pass, 82 terms through n = 4437 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Computes the n‑th term of OEIS sequence A141663. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for offset 0, defensive return */

    /* Find the row number k such that k(k+1)/2 ≥ n. */
    idx_t k = 1;
    while ((k * (k + 1)) / 2 < n)
        ++k;

    /* Compute the column index m (0‑based). */
    idx_t prev = (k - 1) * k / 2;
    idx_t m = n - prev - 1;      /* 0 ≤ m < k */

    /* Retrieve the k‑th prime. */
    seq_t prime = (seq_t)get_prime(k);

    /* Compute m² in the same integer type. */
    seq_t sq = (seq_t)m * (seq_t)m;

    /* Return the absolute difference without using <math.h>. */
    return (prime > sq) ? (prime - sq) : (sq - prime);
}
