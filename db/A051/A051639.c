/*
 * A051639  Concatenation of 3^k, k = 0,..,n.
 * https://oeis.org/A051639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = concat_{k=0}^{n} 3^k (decimal concatenation of 3^0, 3^1,
 *            …, 3^n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"      /* seq_t pow10(idx_t e); */
#include "../utils/digit_len.h"  /* int digit_len(seq_t x); */

seq_t a(idx_t n)
{
    seq_t a_val = 0;   /* current concatenated value a(k) */
    seq_t term  = 1;   /* current power 3^k, starting with k=0 */

    for (idx_t k = 0; k <= n; ++k) {
        /* shift = 10^{digits(term)} */
        seq_t shift = pow10((idx_t)digit_len(term));

        /* a(k) = a(k-1) * shift + term */
        a_val = a_val * shift + term;

        /* prepare next power of 3 */
        term *= 3;
    }

    return a_val;
}
