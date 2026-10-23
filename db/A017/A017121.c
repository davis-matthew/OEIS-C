/*
 * A017121  a(n) = (8*n + 4)^9.
 * https://oeis.org/A017121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 4)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 16, u128 n <= 2388,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = n;            /* convert index to seq_t type */
    x = x * (seq_t)8 + (seq_t)4;  /* compute 8*n + 4 */

    seq_t result = (seq_t)1;    /* accumulate power */
    for (int i = 0; i < 9; ++i) {
        result *= x;   
    }
    return result;
}
