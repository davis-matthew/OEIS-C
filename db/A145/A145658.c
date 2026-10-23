/*
 * A145658  a(n) = numerator of polynomial of genus 1 and level n for m = 3.
 * https://oeis.org/A145658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of sum_{d=1..n-1} 3^(n-d)/d
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t N = 0;   /* current numerator */
    seq_t D = 1;   /* current denominator */
    seq_t p = 1;   /* 3^0 */

    for (idx_t t = 1; t <= n - 1; ++t) {
        /* compute 3^t */
        p *= (seq_t)3;

        /* term = 3^t / (n - t) */
        seq_t term_num = p;
        seq_t term_den = (seq_t)(n - t);

        /* add to the current fraction */
        seq_t newN = N * term_den + term_num * D;
        seq_t newD = D * term_den;
        seq_t g = gcd(newN, newD);      /* reduce fraction */
        N = newN / g;
        D = newD / g;
    }

    return N;   /* numerator of the final reduced fraction */
}
