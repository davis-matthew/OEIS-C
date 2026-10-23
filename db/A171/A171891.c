/*
 * A171891  1 and all numbers >= 10.
 * https://oeis.org/A171891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(n) = n + 8 for n >= 2
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset is 1; behaviour for n < 1 is irrelevant */
    if (n <= 1)
        return 1;

    /* direct computation in seq_t to avoid intermediate overflows */
    return ((seq_t)n) + ((seq_t)8);
}
