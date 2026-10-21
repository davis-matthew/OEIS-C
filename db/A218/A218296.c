/*
 * A218296  Expansion of e.g.f. Sum_{n>=0} n^n * cosh(n*x) * x^n/n!.
 * https://oeis.org/A218296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (n-2*k)^n * binomial(n,2*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t bin = 1; /* binomial(n,0) */

    for (idx_t j = 0; j <= n; j += 2) {
        seq_t pow_val = big_pow((seq_t)(n - j), (seq_t)n);
        sum += bin * pow_val;

        /* update to binomial(n, j+2) */
        if (n - j >= 2) {
            bin = bin * (seq_t)(n - j);
            bin = bin / (seq_t)(j + 1);
            bin = bin * (seq_t)(n - j - 1);
            bin = bin / (seq_t)(j + 2);
        }
    }
    return sum;
}
