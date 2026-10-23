/*
 * A037867  a(n)=(number of digits <=3)-(number of digits >3) in base 8
 *          representation of n.
 * https://oeis.org/A037867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = Σ over octal digits d of n of (d <= 3 ? +1 : -1)
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t x = n;
    while (x > 0) {
        unsigned d = (unsigned)(x & 7);   /* least significant octal digit */
        sum += (d <= 3) ? 1 : -1;
        x >>= 3;                         /* divide by 8 */
    }
    return sum;
}
