/*
 * A045892  Solutions s to the equation 1=s*prime(n)+t*prime(n+1) with |s| as
 *          small as possible.
 * https://oeis.org/A045892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 56, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A045892
 *  Definition:
 *  a(n) is the integer s with minimal absolute value satisfying
 *  1 = s*prime(n) + t*prime(n+1) for some integer t.
 *  Equivalent to the modular inverse of prime(n) modulo prime(n+1)
 *  rounded to the nearest integer to 0.
 */

#include <stdint.h>
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

seq_t a(idx_t n)
{
    /*  n is 1‑based.  Return 0 for n < 1 (unused case).  */
    if (n < 1) return (seq_t)0;

    uint32_t p = get_prime(n);     /* prime(n)   */
    uint32_t q = get_prime(n + 1); /* prime(n+1) */

    /* Extended Euclidean algorithm to find the inverse of p modulo q.
     * Since p and q are consecutive primes, gcd(p, q) = 1.  */
    int64_t a0 = (int64_t)p;
    int64_t b0 = (int64_t)q;
    int64_t x0 = 1, y0 = 0;
    int64_t x1 = 0, y1 = 1;
    while (b0 != 0) {
        int64_t qv = a0 / b0;
        int64_t r  = a0 % b0;

        int64_t x2 = x0 - qv * x1;
        int64_t y2 = y0 - qv * y1;

        a0 = b0;  b0 = r;
        x0 = x1;  x1 = x2;
        y0 = y1;  y1 = y2;
    }

    /* x0 now satisfies p*x0 + q*y0 = 1, so x0 ≡ p⁻¹ (mod q).  */
    int64_t inv = x0 % (int64_t)q;
    if (inv < 0) inv += (int64_t)q;   /* normalize to [0, q-1] */

    /* Choose the representative with smallest absolute value.  */
    int64_t half = (int64_t)q / 2;
    int64_t s = (inv > half) ? inv - (int64_t)q : inv;

    return (seq_t)s;
}
