/*
 * A115714  Periodic {1,-1,-3,0,1,-5,1,0,-3,-1,1,-4}.
 * https://oeis.org/A115714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A115714: periodic of length 12, pattern:
   1, -1, -3, 0, 1, -5, 1, 0, -3, -1, 1, -4 */
seq_t a(idx_t n)
{
    static const seq_t pattern[12] = {
        1,  -1, -3,  0,
        1,  -5,  1,  0,
        -3, -1,  1, -4
    };
    /* compute the position within the 12‑term cycle */
    idx_t idx = n % 12;
    return pattern[idx];
}
