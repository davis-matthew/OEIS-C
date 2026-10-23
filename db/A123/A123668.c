/*
 * A123668  Smallest pandigital palindrome in base n, with a(1) = 1.
 * https://oeis.org/A123668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 1 + n^(2n-2) + (n-1)n^(n-1) * + Σ_{i=2}^{n-2}
 *            i*(n^(2n-2-i)+n^i) (for n > 2) * a(1) = 1, a(2) = 5
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)5;

    /* Convert n to seq_t once */
    seq_t n_seq = (seq_t)n;

    /* n^(2n-2) */
    seq_t high_pow = seq_pow(n_seq, 2 * n - 2);

    /* 1 + n^(2n-2) */
    seq_t sum = high_pow + (seq_t)1;

    /* (n-1)*n^(n-1) */
    seq_t mid_pow = seq_pow(n_seq, n - 1);
    sum += ((seq_t)(n - 1)) * mid_pow;

    /* Prepare for summation over i = 2 .. n-2
     * left  = n^(2n-2-i), right = n^i
     */
    seq_t left  = high_pow;   /* start with n^(2n-2) for i = 0 */
    seq_t right = (seq_t)1;    /* start with n^0 */

    for (idx_t i = 1; i <= n - 2; ++i) {
        left  /= n_seq;        /* n^(2n-2-i) for current i */
        right *= n_seq;        /* n^i                  */

        if (i >= 2) {
            seq_t term = (seq_t)i * (left + right);
            sum += term;
        }
    }

    return sum;
}
