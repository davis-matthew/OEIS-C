/*
 * A133015  Concatenation of next n positive triangular numbers.
 * https://oeis.org/A133015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concat_{k = n*(n-1)/2 + 1}^{n*(n+1)/2} T_k, where T_k =
 *            k(k+1)/2 is the k‑th triangular number
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety for offset < 1 */

    seq_t result = 0;
    idx_t start = n * (n - 1) / 2 + 1;
    idx_t end   = n * (n + 1) / 2;

    for (idx_t k = start; k <= end; ++k)
    {
        /* triangular number T_k = k(k+1)/2 */
        seq_t t = (seq_t)k * ((seq_t)k + 1) / 2;

        /* number of decimal digits of t */
        size_t d = 0;
        seq_t tmp = t;
        do {
            ++d;
            tmp /= (seq_t)10;
        } while (tmp != 0);

        /* compute 10^d */
        seq_t pow10 = (seq_t)1;
        for (size_t i = 0; i < d; ++i)
            pow10 *= (seq_t)10;

        /* concatenate: result = result * 10^d + t */
        result = result * pow10 + t;
    }

    return result;
}
