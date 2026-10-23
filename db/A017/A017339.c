/*
 * A017339  a(n) = (10*n + 5)^11.
 * https://oeis.org/A017339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 5)^11
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 5, u128 n <= 317,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t to avoid overflow on integer types. */
    seq_t base = 10 * (seq_t)n + 5;
    seq_t res  = 1;

    /* 11-th power by repeated multiplication (exponent is constant). */
    for (int i = 0; i < 11; ++i)
        res *= base;

    return res;
}
