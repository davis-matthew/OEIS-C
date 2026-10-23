/*
 * A024026  a(n) = 3^n - n^3.
 * https://oeis.org/A024026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - n^3
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper pow_int is provided by the build environment.
   It computes base ^ exp where base is a seq_t and exp is an idx_t. */
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* 3^n */
    seq_t pow3 = pow_int((seq_t)3, n);

    /* n^3 */
    seq_t n_val   = (seq_t)n;
    seq_t n_cubed = pow_int(n_val, (idx_t)3);

    return pow3 - n_cubed;
}
