/*
 * A096384  a(n) is the area of the triangle with sides prime(n), prime(n+2)
 *          and prime(n+4), rounded down to the nearest integer.
 * https://oeis.org/A096384
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sqrt( P*(P-2p1)*(P-2p2)*(P-2p3) ) / 4 ) * where p1
 *            = prime(n), p2 = prime(n+2), p3 = prime(n+4), * P = p1+p2+p3.
 *            This is the integer part of the area of the * triangle with
 *            sides p1, p2, p3
 * Range:     u8 none, u16 n <= 4, u32 n <= 31, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 58 terms through n = 962 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"

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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;

    /* Retrieve the three primes. */
    seq_t p1 = (seq_t)get_prime(n);
    seq_t p2 = (seq_t)get_prime(n + 2);
    seq_t p3 = (seq_t)get_prime(n + 4);

    /* Semi‑perimeter as a rational number with denominator 1/2.  The
     * exact Heron product simplifies to:  P * (P-2p1) * (P-2p2) * (P-2p3) / 16.
     * Thus sqrt(product) = sqrt(N) / 4 where N = P*(P-2p1)*(P-2p2)*(P-2p3). */
    seq_t P  = p1 + p2 + p3;
    seq_t f1 = P - (p1 << 1); /* P - 2*p1 */
    seq_t f2 = P - (p2 << 1); /* P - 2*p2 */
    seq_t f3 = P - (p3 << 1); /* P - 2*p3 */

    seq_t N = P * f1 * f2 * f3;          /* 64‑bit * 64‑bit *… fits in seq_t (128‑bit). */
    seq_t sqrtN = floor_sqrt(N);          /* floor of sqrt(N) */
    return sqrtN >> 2;                    /* divide by 4, integer truncation */
}
