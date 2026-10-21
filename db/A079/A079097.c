/*
 * A079097  Mix odd numbers and squares.
 * https://oeis.org/A079097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+1 if n is even * a(n) = ((n+1)/2)² if n is odd * (This
 *            matches the sequence A079097: odd indices give perfect squares,
 *            * even indices give consecutive integers.)
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Even n: a = n+1 */
    if ((n & 1) == 0) {
        return (seq_t)n + 1;
    }

    /* Odd n: a = ((n+1)/2)^2 */
    seq_t t = ((seq_t)n + 1) / 2;  /* (n+1)/2 fits in seq_t for any n that fits in idx_t */
    return t * t;
}
