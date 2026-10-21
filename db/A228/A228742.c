/*
 * A228742  Number of arrays of the median of three adjacent elements of some
 *          length 7 0..n array.
 * https://oeis.org/A228742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*(19*n^4+186*n^3+349*n^2+226*n+60)/60
 * Range:     u8 none, u16 n <= 3, u32 n <= 44, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 – return 0 only for safety */
    seq_t n_seq = (seq_t)n;          /* promote index to internal type */
    seq_t n2 = n_seq * n_seq;        /* n^2 */
    seq_t n3 = n2 * n_seq;           /* n^3 */
    seq_t n4 = n3 * n_seq;           /* n^4 */
    seq_t poly = 19 * n4 + 186 * n3 + 349 * n2 + 226 * n_seq + 60;
    seq_t num = (n_seq + 1) * poly;
    return num / 60;                  /* exact division */
}
