/*
 * A175239  Number of AP divisors of n.
 * https://oeis.org/A175239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   ugcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Number of AP divisors of n.
 * Implementation based on the definition: k is an AP divisor of n
 * if n can be expressed as the sum of an arithmetic progression of
 * length k with positive integer terms.  Length 1 is always
 * included.  For k ≥ 2 we consider two cases:
 *   1) common difference d = 0  →  k must divide n.
 *   2) common difference d > 0  →  we search for a positive integer d
 *      such that n = k*a + d*k*(k-1)/2 with a ≥ 1.
 * The search for d > 0 reduces to solving a linear congruence
 *   n - d*k*(k-1)/2 ≡ 0 (mod k),
 * and then checking that d is small enough so that the term
 *   a = (n - d*k*(k-1)/2)/k  ≥ 1.
 *
 * The algorithm runs in O(√n) time: we loop over k = 2 … n,
 * but for d > 0 we only need to examine k where
 * k(k-1)/2 < n, which limits k to O(√n).  The function is
 * safe for large n (inside 64‑bit limits) and uses only integer
 * arithmetic, as required.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/ugcd_uint64.h"

/* Compute greatest common divisor of two 64‑bit unsigned integers */

/* Extended Euclidean algorithm: find inverse of a modulo m (gcd(a,m)=1) */
static inline uint64_t modinv_uint64(uint64_t a, uint64_t m)
{
    int64_t t0 = 0, t1 = 1;
    int64_t r0 = m, r1 = a;
    while (r1 != 0) {
        uint64_t q = r0 / r1;
        int64_t r2 = r0 - (int64_t)q * r1;
        int64_t t2 = t0 - (int64_t)q * t1;
        r0 = r1; r1 = r2;
        t0 = t1; t1 = t2;
    }
    if (r0 != 1) return 0;          /* inverse does not exist */
    if (t0 < 0) t0 += m;
    return (uint64_t)t0;
}

/* Return the number of AP divisors of n (OEIS A175239). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* outside the offset, return 0 */

    seq_t count = 1;               /* k = 1 always a divisor */

    /* iterate k from 2 to n */
    for (idx_t k = 2; k <= n; ++k) {

        /* case d = 0 : all parts equal */
        if ((n % k) == 0) {
            ++count;
            continue;              /* skip d > 0 check */
        }

        /* case d > 0 : need k(k-1)/2 < n to have any positive d */
        __uint128_t m128 = (__uint128_t)k * (k - 1) / 2;
        if (m128 >= n) continue;   /* impossible to have d > 0 */

        idx_t m   = (idx_t)m128;    /* safe conversion: m < n <= UINT64_MAX */
        idx_t r   = n % k;
        idx_t g   = ugcd_uint64(m, k);
        if (r % g != 0) continue;  /* congruence unsolvable */

        idx_t m1  = m / g;          /* reduced coefficients */
        idx_t k1  = k / g;
        idx_t r1  = r / g;

        uint64_t inv = modinv_uint64(m1, k1);   /* inverse exists because m1, k1 coprime */
        if (inv == 0) continue;                 /* safety, should not happen */

        uint64_t x = (uint64_t)((__uint128_t)r1 * inv % k1); /* minimal solution t modulo k1 */

        uint64_t t_min = (x == 0) ? k1 : (uint64_t)x;        /* ensure d >= 1 */
        uint64_t limit = (n - 1) / m;                        /* largest d with n - m*d > 0 */

        if (t_min <= limit) ++count;                        /* at least one valid d */
    }

    return count;
}
