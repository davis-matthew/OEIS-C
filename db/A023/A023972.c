/*
 * A023972  First bit in fractional part of binary expansion of 5th root of n.
 * https://oeis.org/A023972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if the first binary digit after the point of * the
 *            fractional part of the fifth root of n is 1, otherwise 0. *
 *            Equivalently: let k = ⌊n^(1/5)⌋. Then a(n) = 1 iff * n ≥ (k +
 *            1/2)^5
 * Range:     u8 n <= 7, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow5(seq_t x) {
    seq_t x2 = x * x;     // x^2
    seq_t x4 = x2 * x2;   // x^4
    return x4 * x;        // x^5
}

/* compute floor(n^(1/5)) for a nonnegative n given as seq_t */
static seq_t floor_fifth_root(seq_t n) {
    seq_t lo = 0;
    seq_t hi = 1;
    /* increase hi until hi^5 > n */
    while (pow5(hi) <= n) hi <<= 1;
    /* binary search in (lo, hi] */
    while (hi - lo > 1) {
        seq_t mid = (lo + hi) >> 1;
        if (pow5(mid) <= n) lo = mid;
        else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but safe */
    seq_t tn = (seq_t)n;
    seq_t k = floor_fifth_root(tn);

    /* Compute N = 32*k^5 + 80*k^4 + 80*k^3 + 40*k^2 + 10*k + 1 */
    seq_t k2 = k * k;          /* k^2 */
    seq_t k3 = k2 * k;         /* k^3 */
    seq_t k4 = k3 * k;         /* k^4 */
    seq_t k5 = k4 * k;         /* k^5 */

    seq_t N = (seq_t)32 * k5;
    N += (seq_t)80 * k4;
    N += (seq_t)80 * k3;
    N += (seq_t)40 * k2;
    N += (seq_t)10 * k;
    N += (seq_t)1;

    /* Compare 32*n with N */
    seq_t left = (seq_t)32 * tn;
    return (seq_t)(left >= N);
}
