/*
 * A048509  a(n) = T(4,n), array T given by A048505.
 * https://oeis.org/A048509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2+17*n+64)*2^(n-2) - 15 valid for all n >= 0. For n <
 *            4 the first four terms are returned directly. The function works
 *            for any seq_t/idx_t
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 23, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms for n = 0,1,2,3 */
    static const seq_t init[4] = { (seq_t)1, (seq_t)26,
                                   (seq_t)87, (seq_t)233 };

    if (n < 4) {
        return init[(size_t)n];
    }

    /* Compute (n^2 + 17*n + 64) */
    seq_t n_seq   = (seq_t)n;
    seq_t factor  = n_seq * n_seq + (seq_t)17 * n_seq + (seq_t)64;

    /* Compute 2^(n-2) by successive left shifts */
    seq_t pow = (seq_t)1;
    idx_t exp = n - 2;
    for (idx_t i = 0; i < exp; ++i) {
        pow <<= 1; /* multiply by 2 */
    }

    /* Final formula: (factor * pow) - 15 */
    return factor * pow - (seq_t)15;
}
