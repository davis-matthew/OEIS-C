/*
 * A182944  Square array A(i,j), i >= 1, j >= 1, of prime powers prime(i)^j,
 *          by descending antidiagonals.
 * https://oeis.org/A182944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 38, u32 n <= 95, u64 n <= 237,
 *            u128 n <= 639, big any n
 * Validated: standard, pass, 61 terms through n = 7844 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A182944 – Square array of prime powers written along antidiagonals.
   For index n >= 1, let d be the smallest integer with d*(d-1)/2 >= n.
   The antidiagonal d contains the pairs (i, j) with i+j = d and i ranging
   from 1 to d-1.  The n-th term is prime(i)^j where
        i = 1 + (n - (d-1)*(d-2)/2 - 1)
        j = d - i
   This implementation uses get_prime(idx_t) to fetch the i-th prime
   and pow_uint(uint64_t, idx_t) to compute the power.                */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_u64_idx.h"

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

seq_t a(idx_t n)
{
    if (n < 1) return 0;          // undefined for n = 0, but safe guard

    /* Find the smallest d such that d*(d-1)/2 >= n.  Binary search
       on d, using 128‑bit arithmetic to avoid overflow. */
    idx_t low = 1, high = 2;

    while ((__int128)high * (high - 1) / 2 < n)
        high <<= 1;                /* increase upper bound until sufficient */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tot = (__int128)mid * (mid - 1) / 2;
        if (tot >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;                 /* diagonal index */

    /* Number of terms before diagonal d */
    __int128 prev_tot = (__int128)(d - 1) * (d - 2) / 2;
    idx_t offset = n - (idx_t)prev_tot - 1;      /* zero‑based offset within diagonal */

    idx_t i = 1 + offset;           /* row number   (prime index) */
    idx_t j = d - i;                /* column number (exponent)   */

    uint32_t prime_i = get_prime(i);    /* i‑th prime */
    seq_t result = ipow_u64_idx((uint64_t)prime_i, j);

    return result;
}
