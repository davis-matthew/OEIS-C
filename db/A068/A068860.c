/*
 * A068860  a(1) = 1; a(n+1) is the smallest number > a(n) which differs from
 *          it at every digit.
 * https://oeis.org/A068860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = f aba ba... where f = ((n-1) mod 9) + 1 (first digit) ab
 *            = 01 if f is odd else 10 total number of digits = floor((n-1)/9)
 *            + 1 The pattern after the first digit repeats "ab" until the
 *            specified digit count
 * Range:     u8 n <= 20, u16 n <= 42, u32 n <= 85, u64 n <= 172,
 *            u128 n <= 345, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t digits = (n - 1) / 9 + 1;          /* total digits of a(n) */
    idx_t f       = ((n - 1) % 9) + 1;      /* first digit */

    /* digits of the repeating pattern */
    int a_digit = (f & 1) ? 0 : 1; /* 0 if f is odd, else 1 */
    int b_digit = (f & 1) ? 1 : 0; /* 1 if f is odd, else 0 */

    seq_t val = 0;
    val = val * 10 + (seq_t)f; /* first digit */

    for (idx_t i = 0; i < digits - 1; ++i)
    {
        int d = (i & 1) ? b_digit : a_digit;
        val = val * 10 + (seq_t)d;
    }
    return val;
}
