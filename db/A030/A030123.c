/*
 * A030123  Most likely total for a roll of n 6-sided dice, choosing the
 *          smallest if there is a choice.
 * https://oeis.org/A030123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 (n=0), 1 (n=1), floor(7*n/2) for n >= 2
 * Range:     u8 n <= 73, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n >= 2: a(n) = (7 * n) / 2, integer division truncates toward zero, which is floor for positive integers. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;
    return ((seq_t)n * 7) / 2;
}
