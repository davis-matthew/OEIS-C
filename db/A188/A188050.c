/*
 * A188050  a(n) = A016755(n) - A001845(n).
 * https://oeis.org/A188050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10/3)*n*(n+1)*(2n+1)
 * Range:     u8 n <= 2, u16 n <= 20, u32 n <= 863, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a_ = (seq_t)n;          /* a = n                     */
    seq_t b = a_ + 1;             /* b = n + 1                 */
    seq_t c = (a_ << 1) + 1;      /* c = 2n + 1                */

    /* Divide one of the factors by 3 (always possible) to keep values small */
    if (a_ % 3 == 0)   a_ /= 3;
    else if (b % 3 == 0)  b /= 3;
    else                 c /= 3;

    /* Multiply the remaining factors and multiply by 10 */
    a_ *= b;      /* a_ = a * b */
    a_ *= c;      /* a_ = a * b * c */
    a_ *= 10;     /* a_ = 10 * a * b * c */

    return a_;
}
