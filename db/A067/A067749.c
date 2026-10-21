/*
 * A067749  Numbers k such that k and 3^k end with the same two digits.
 * https://oeis.org/A067749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 100*n - 13 (for n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1; return 0 or undefined for n<1 */
        return 0;

    /* Use seq_t for all intermediate arithmetic to keep the result exact
       as long as it fits in seq_t. */
    return (seq_t)n * (seq_t)100 - (seq_t)13;
}
