/*
 * A124432  Denominator of Sum_{k=1..n} 1/H(k), where H(k) = Sum_{j=1..k} 1/j
 *          is the k-th harmonic number.
 * https://oeis.org/A124432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of sum_{k=1..n} 1/H(k) where H(k) =
 *            sum_{j=1..k} 1/j
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;            /* a(0) = 1 */

    seq_t H_num = 1, H_den = 1;    /* Harmonic number H(1) = 1/1 */
    seq_t S_num = 0, S_den = 1;    /* Sum initialized to 0/1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        if (k > 1)
        {
            seq_t k_seq = (seq_t)k;
            seq_t new_num = H_num * k_seq + H_den;    /* a*k + b */
            seq_t new_den = H_den * k_seq;            /* b*k */
            seq_t g = gcd(new_num, new_den);
            H_num = new_num / g;
            H_den = new_den / g;
        }

        /* term = 1/H_k = H_den / H_num */
        seq_t term_num = H_den;
        seq_t term_den = H_num;

        seq_t g = gcd(S_den, term_den);
        seq_t factor1 = term_den / g;
        seq_t factor2 = S_den   / g;

        seq_t new_num = S_num * factor1 + term_num * factor2;
        seq_t new_den = (S_den / g) * term_den;

        seq_t g2 = gcd(new_num, new_den);
        S_num = new_num / g2;
        S_den = new_den / g2;
    }

    return S_den;
}
