/*
 * A234714  Numerator of sum_{k=1..n} 1/(k*H(k)) where H(k) is the harmonic
 *          number H(k) = sum_{j=1..n} 1/j.
 * https://oeis.org/A234714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numerator of sum_{k=1..n} 1/(k*H(k)), H(k)=sum_{j=1..k}
 *            1/j
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute a(n) : numerator of the sum of 1/(k*H(k)) for k=1..n.
   Uses only integer arithmetic, no floating point.  Works for any seq_t/idx_t
   provided by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* running sum (numerator / denominator) */
    seq_t sum_num = 0;
    seq_t sum_den = 1;

    /* current harmonic number H(k) as reduced fraction H_num/H_den */
    seq_t H_num = 1;
    seq_t H_den = 1;

    for (idx_t k = 1; k <= n; ++k) {
        /* term = 1/(k*H(k)) = H_den / (k*H_num) */
        seq_t term_num = H_den;
        seq_t term_den = (seq_t)k * H_num;
        seq_t g = gcd(term_num, term_den);
        term_num /= g;
        term_den /= g;

        /* sum += term */
        seq_t new_num = sum_num * term_den + term_num * sum_den;
        seq_t new_den = sum_den * term_den;
        g = gcd(new_num, new_den);
        new_num /= g;
        new_den /= g;
        sum_num = new_num;
        sum_den = new_den;

        /* update harmonic number to H(k+1) for next iteration */
        if (k != n) {
            seq_t newH_num = H_num * (k + 1) + H_den;
            seq_t newH_den = H_den * (k + 1);
            g = gcd(newH_num, newH_den);
            newH_num /= g;
            newH_den /= g;
            H_num = newH_num;
            H_den = newH_den;
        }
    }
    return sum_num;   /* numerator of the reduced sum */
}
