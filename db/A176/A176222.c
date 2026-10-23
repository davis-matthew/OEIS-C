/*
 * A176222  a(n) = (n^2 - 3*n + 1 + (-1)^n)/2.
 * https://oeis.org/A176222
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - 3*n + 1 + (-1)^n)/2
 * Range:     u8 n <= 24, u16 n <= 363, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The offset is 3; for n < 3 the value is undefined, but we return 0 */
    if (n < 3)
        return 0;

    idx_t n2 = n * n;          /* n² – fits in idx_t */
    if (n & 1)                 /* odd n: (-1)^n = -1 */
        return (seq_t)((n2 - 3 * n) / 2);
    else                      /* even n: (-1)^n = 1  */
        return (seq_t)((n2 - 3 * n + 2) / 2);
}
