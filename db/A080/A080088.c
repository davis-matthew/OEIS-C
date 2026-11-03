/*
 * A080088  Let r+i*s be the sum of the first-quadrant Gaussian primes
 *          dividing prime(n); sequence gives r values.
 * https://oeis.org/A080088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 2610, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9656 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A080088: sum of r+s of the first‑quadrant Gaussian
   primes dividing the n‑th ordinary prime
   (i.e., a(p) = p for p ≡ 3 (mod 4), otherwise a(p) = a + b
   where p = a² + b² with a ≥ b > 0). */
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
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
    if (n == 0) return (seq_t)0;          /* safety for index 0 */

    seq_t p = (seq_t)get_prime(n);        /* the n‑th ordinary prime */

    if (p == (seq_t)2)                    /* 2 = (1+i)² → r+s = 2 */
        return (seq_t)2;

    /* For prime ≡ 3 (mod 4): it is Gaussian prime on the real axis,
       r+s equals the prime itself. */
    if ((p % (seq_t)4) == (seq_t)3)
        return p;

    /* Prime ≡ 1 (mod 4): it splits as a² + b² with a > b > 0.
       Find the representation and return a + b. */
    seq_t sqrtp = floor_sqrt(p);
    for (seq_t a_val = sqrtp; a_val > (seq_t)0; --a_val) {
        seq_t a2 = a_val * a_val;
        seq_t b2 = p - a2;
        seq_t b_val = floor_sqrt(b2);
        if (b_val > (seq_t)0 && b_val * b_val == b2) {
            /* a_val ≥ b_val > 0 as required. */
            return a_val + b_val;
        }
    }

    /* Should never reach here for prime p ≥ 2. */
    return (seq_t)0;
}
