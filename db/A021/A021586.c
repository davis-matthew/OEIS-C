/*
 * A021586  Decimal expansion of 1/582.
 * https://oeis.org/A021586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = n-th decimal digit of 1/582 (starting at a(0)=0)
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;         /* remainder after decimal point start */
    seq_t d = 0;         /* current digit */
    for (idx_t i = 0; i <= n; ++i) {
        r = r * ((seq_t)10);
        d = r / ((seq_t)582);   /* digit */
        r = r % ((seq_t)582);   /* new remainder */
    }
    return d;
}
