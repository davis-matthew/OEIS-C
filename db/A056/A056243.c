/*
 * A056243  Third diagonal of triangle A056242.
 * https://oeis.org/A056243
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-6)*(32-35*n+9*n^2), n >= 3
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 3.  For safety we return 0 for smaller indices. */
    if (n < 3) return 0;

    seq_t nn = (seq_t)n;
    seq_t val = nn * nn * 9;   /* 9*n^2 */
    val -= nn * 35;            /* subtract 35*n */
    val += 32;                 /* add 32 */

    if (n >= 6) {
        val <<= (n - 6);       /* left shift by (n-6) = multiply by 2^(n-6) */
    } else {
        val >>= (6 - n);       /* right shift by (6-n) = divide by 2^(6-n) */
    }

    return val;
}
