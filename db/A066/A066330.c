/*
 * A066330  Binary string which equals n when 1's, 2's, 4's and 8's bits have
 *          weights 1, 2, 4, 5 respectively, while the other bits have their
 *          usual weights. -1 if no such string exists.
 * https://oeis.org/A066330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 31, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the value of the sequence A066330.
    For every n ≥ 0, let m be n or n+3 as follows:
      m = n+3  if (n mod 16) is between 5 and 12 inclusive,
      m = n    otherwise.
    Let w be the weighted sum of m, where bit 3 is weighted 5
    and all other bits are weighted as their powers of 2.
    If w equals n, a(n) is the decimal integer whose digits
    are the binary digits of m; otherwise a(n) = -1.  */
seq_t a(idx_t n)
{
    /* choose candidate m */
    seq_t m;
    if ((n & 15ULL) >= 5 && (n & 15ULL) <= 12)
        m = (seq_t)n + 3;
    else
        m = (seq_t)n;

    /* compute weighted sum of m: subtract 3 if bit 3 is set */
    seq_t bw = (m >> 3) & 1;            /* 0 or 1 */
    seq_t w = m - 3 * bw;
    if (w != (seq_t)n)
        return (seq_t)-1;               /* no representation */

    /* convert binary m into its decimal digit representation */
    if (m == 0)
        return (seq_t)0;

    /* find number of bits in m */
    int bits = 0;
    seq_t tmp = m;
    while (tmp > 0) {
        bits++;
        tmp >>= 1;
    }

    /* build the decimal number digit by digit */
    seq_t result = 0;
    for (int i = bits - 1; i >= 0; --i) {
        seq_t bit = (m >> i) & 1;
        result = result * 10 + bit;
    }
    return result;
}
