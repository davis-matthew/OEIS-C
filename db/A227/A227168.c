/*
 * A227168  a(n) = gcd(2*n, n*(n+1)/2)^2.
 * https://oeis.org/A227168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = gcd(2*n, n*(n+1)/2)^2
 * Range:     u8 n <= 10, u16 n <= 130, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* simple Euclidean gcd on idx_t (uint64_t) */
static inline idx_t gcd_uint64(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* outside the offset, defined arbitrarily */

    /* compute 2n and the triangular number in 64‑bit domain */
    idx_t two_n = n + n;
    idx_t tri = n * (n + 1) / 2;

    /* gcd on 64‑bit values, then cast to seq_t */
    idx_t g = gcd_uint64(two_n, tri);
    seq_t res = (seq_t)g * (seq_t)g;
    return res;
}
