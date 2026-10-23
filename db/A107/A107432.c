/*
 * A107432  Column 3 of array in A107431.
 * https://oeis.org/A107432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n + floor((n-1)/2) for n ≠ 2,4; a(2)=1, a(4)=4
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2)
        return (seq_t)1;
    if (n == 4)
        return (seq_t)4;

    /* n + floor((n-1)/2)  */
    return (seq_t)n + (seq_t)((n - 1) / 2);
}
