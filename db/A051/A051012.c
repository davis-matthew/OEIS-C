/*
 * A051012  Denominator of average of number of steps in Euclidean algorithm
 *          for all gcd(m,n) with 0 <= m < n.
 * https://oeis.org/A051012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n / gcd( Σ_{m=0}^{n-1} steps(m,n) , n ) * where
 *            steps(m,n) is the number of recursive calls of the Euclidean *
 *            algorithm (division version) needed to compute gcd(m,n). * * The
 *            function uses only the built‑in operators of the language. * It
 *            avoids any external headers and implements an inli...
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* inline Euclidean step count using division */
static inline idx_t euclid_steps(idx_t a, idx_t b)
{
    idx_t cnt = 0;
    while (b != 0) {
        cnt++;
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return cnt;
}

/* Euclidean algorithm GCD – works for any seq_t |
 * Using idx_t for the calculation ensures no overflow
 * on the temporary variables even when seq_t is tiny.  */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Main OEIS sequence function */
seq_t a(idx_t n)
{
    /* out of sequence bounds – conventionally zero */
    if (n == 0) return 0;

    /* compute the sum of Euclidean steps for m = 1 … n-1   */
    idx_t sum = 0;
    for (idx_t m = 1; m < n; ++m) {
        sum += euclid_steps(n, m);
    }

    /* denominator of the average = n / gcd(sum , n)        */
    idx_t g = gcd_idx(sum, n);
    return (seq_t)(n / g);
}
