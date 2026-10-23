/*
 * A017135  a(n) = (8*n + 5)^11.
 * https://oeis.org/A017135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 5)^11
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 6, u128 n <= 397,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)8 * n + 5;
    seq_t result = base;
    /* multiply base by itself 10 more times to get the 11th power */
    for (int i = 1; i < 11; ++i)
        result *= base;
    return result;
}
