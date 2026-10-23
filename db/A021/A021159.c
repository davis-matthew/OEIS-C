/*
 * A021159  Decimal expansion of 1/155.
 * https://oeis.org/A021159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth decimal digit of 1/155 by repeated long division. */
seq_t a(idx_t n)
{
    seq_t rem = 1;
    for (idx_t i = 0; i <= n; ++i)
    {
        rem *= 10;
        seq_t d = rem / ((seq_t)155);
        rem %= (seq_t)155;
        if (i == n)
            return d;
    }
    return 0; /* unreachable */
}
