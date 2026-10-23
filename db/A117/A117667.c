/*
 * A117667  a(n) = n^n - n^(n-1) - n^(n-2) - n^(n-3) - ... - n^3 - n^2 - n.
 * https://oeis.org/A117667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n - n^(n-1) - n^(n-2) - ... - n^2 - n, with a(0)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;

    seq_t base = (seq_t)n;
    seq_t pow_n_n        = big_pow(base, (seq_t)n);
    seq_t pow_n_n_minus1 = big_pow(base, (seq_t)(n - 1));

    seq_t numerator = (pow_n_n_minus1 - (seq_t)1) * base;
    seq_t sum = numerator / ((seq_t)(n - 1));

    return pow_n_n - sum;
}
