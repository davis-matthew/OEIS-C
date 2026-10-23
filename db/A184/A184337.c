/*
 * A184337  a(n) is the integer whose decimal representation consists of n 8's
 *          followed by n 1's.
 * https://oeis.org/A184337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (8*100^n - 7*10^n - 1)/9; i.e. a decimal number with n
 *            8’s followed by n 1’s
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t res = (seq_t)0;

    /* Append n 8's */
    for (idx_t i = 0; i < n; ++i)
        res = res * (seq_t)10 + (seq_t)8;

    /* Append n 1's */
    for (idx_t i = 0; i < n; ++i)
        res = res * (seq_t)10 + (seq_t)1;

    return res;
}
