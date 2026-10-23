/*
 * A187989  Number of nondecreasing arrangements of 5 numbers x(i) in
 *          -(n+3)..(n+3) with the sum of sign(x(i))*2^|x(i)| zero.
 * https://oeis.org/A187989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n^2 + 15*n + 19
 * Range:     u8 n <= 7, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of nondecreasing arrangements of 5 numbers x(i) in -(n+3)..(n+3)
 * with sum(sign(x(i))*2^|x(i)|) = 0. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;            // convert idx_t to seq_t
    seq_t res = N * N * ((seq_t)2) + N * ((seq_t)15) + ((seq_t)19);
    return res;
}
