/*
 * A144077  a(n) = z(n^2,n) with z(x,y) = if x>y then z(x-y,y+1) else y.
 * https://oeis.org/A144077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = z(n^2, n) where z(x, y) = (x > y) ? z(x − y, y + 1) : y
 * Range:     u8 n <= 15, u16 n <= 518, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * (seq_t)n;   /* x = n^2  */
    seq_t y = n;                     /* y = n    */
    while (x > y) {
        x -= y;
        y += 1;
    }
    return y;
}
