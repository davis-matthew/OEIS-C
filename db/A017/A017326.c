/*
 * A017326  a(n) = (10*n + 4)^10.
 * https://oeis.org/A017326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 4)^10
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 8, u128 n <= 712,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ten  = (seq_t)10;
    seq_t four = (seq_t)4;
    seq_t base = ten * n + four;

    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < 10; ++i)
        result *= base;

    return result;
}
