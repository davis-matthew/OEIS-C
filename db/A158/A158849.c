/*
 * A158849  a(10n+m) is the integer with n+m concatenations of the digit m in
 *          base 10, 0<=m<=9.
 * https://oeis.org/A158849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(10*n+m) = integer consisting of (n+m) copies of digit m (base
 *            10)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 18, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* When m is 0 the resulting integer is 0. */

seq_t a(idx_t n)
{
    /* decompose n into the block index n and the digit m */
    idx_t n_part = n / 10;
    int m = (int)(n % 10);

    if (m == 0)
        return 0;                /* 0 repeated any number of times is 0 */

    /* number of repeated digits */
    idx_t rep = n_part + (idx_t)m;

    /* build the number by repeated *10 + digit */
    seq_t val = 0;
    for (idx_t i = 0; i < rep; ++i)
        val = val * 10 + (seq_t)m;

    return val;
}
