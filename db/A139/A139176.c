/*
 * A139176  a(n) = (n! - 6)/6.
 * https://oeis.org/A139176
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n! - 6)/6 for n >= 3
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)                 /* offset is 3; values below are undefined */
        return (seq_t)0;

    seq_t fact = 1;
    int removed2 = 0, removed3 = 0;

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t val = (seq_t)i;

        if (!removed2 && (val & 1) == 0)  /* first even factor -> divide by 2 */
        {
            val >>= 1;
            removed2 = 1;
        }

        if (!removed3 && val % 3 == 0)    /* first multiple of 3 -> divide by 3 */
        {
            val /= 3;
            removed3 = 1;
        }

        fact *= val;                      /* accumulate the product */
    }

    /* after the loop fact == n! / 6; subtract 1 to get (n! - 6)/6 */
    return fact - 1;
}
