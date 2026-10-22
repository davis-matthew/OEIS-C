/*
 * A000125  Cake numbers: maximal number of pieces resulting from n planar
 *          cuts through a cube (or cake): C(n+1,3) + n + 1.
 * https://oeis.org/A000125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = C(n+1,3) + n + 1 = (n+1)*n*(n-1)/6 + n + 1
 * Range:     u8 n <= 11, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t x, y, z;
    seq_t r;
    if (n < 2)
        return (seq_t)n + 1;
    x = n + 1;
    y = n;
    z = n - 1;
    /* divide out the 2 and the 3 before multiplying, to keep intermediates small */
    if (x % 2 == 0) x /= 2; else y /= 2;
    if (x % 3 == 0) x /= 3;
    else if (y % 3 == 0) y /= 3;
    else z /= 3;
    r = (seq_t)x;
    r *= (seq_t)y;
    r *= (seq_t)z;
    r += (seq_t)n;
    r += 1;
    return r;
}
