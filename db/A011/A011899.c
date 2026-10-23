/*
 * A011899  a(n) = floor(n*(n-1)*(n-2)/17).
 * https://oeis.org/A011899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)/17)
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1803, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0,1,2 the product contains a zero factor, so a(n)=0. */
    if (n < 3) {
        return (seq_t)0;
    }

    /* Use seq_t for the intermediate computation.  Casting idx_t to
     * seq_t is safe because seq_t is large enough to hold n for all
     * terms that are checked (see the requirements). */
    seq_t sn   = (seq_t)n;
    seq_t prod = sn * (sn - 1) * (sn - 2);
    prod /= (seq_t)17;   /* integer division truncates, giving floor */
    return prod;
}
