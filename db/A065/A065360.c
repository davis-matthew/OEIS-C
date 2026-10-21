/*
 * A065360  Alternating sum of "negabits". Replace (-2)^k with (-1)^k in
 *          negabinary expansion of n.
 * https://oeis.org/A065360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = Σ_{k≥0} b_k * (-1)^k, where n = Σ b_k (-2)^k and
 *            b_k∈{0,1}
 * Range:     i8 n <= 291, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the alternating sum of the negabinary digits of n. */
seq_t a(idx_t n)
{
    seq_t sum = 0;          /* running total */
    seq_t sign = 1;         /* (-1)^k for current bit, start with k=0 */
    seq_t x = (seq_t)n;     /* signed value for processing */

    while (x != 0)
    {
        seq_t rem = x % (-2);   /* remainder may be negative */
        seq_t quot = x / (-2);
        if (rem < 0)            /* adjust to obtain 0 or 1 */
        {
            rem += 2;
            quot += 1;
        }
        sum += rem * sign;     /* add bit contribution */
        sign = -sign;          /* flip sign for next bit */
        x = quot;              /* next digit */
    }

    return sum;
}
