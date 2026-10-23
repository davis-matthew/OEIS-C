/*
 * A174529  a(3n) = a(3n-1)+a(3n-2), a(3n+1) = a(3n)*a(3n-1), a(3n+2) =
 *          a(3n+1)/a(3n).
 * https://oeis.org/A174529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3n) = a(3n-1)+a(3n-2); a(3n+1) = a(3n)*a(3n-1); a(3n+2) =
 *            a(3n+1)/a(3n); Initial values: a(1)=3, a(2)=5, a(3)=8. From the
 *            recurrences it follows that a(3k-1)=5 for all k≥1,
 *            a(3k+1)=5*a(3k), and a(3k)=5*a(3(k-1))+5 (k≥2) with a(3)=8
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 39, u64 n <= 81, u128 n <= 162,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first three terms directly */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)8;

    /* n > 3 */
    idx_t k = n / 3;           /* floor division, number of bulk blocks */
    seq_t b = (seq_t)8;        /* b = a(3) for k == 1 */

    /* compute a(3k) by the simple linear recurrence */
    if (k > 1) {
        for (idx_t i = 2; i <= k; ++i) {
            b = (seq_t)5 * b + (seq_t)5;
        }
    }

    /* choose the appropriate formula based on n mod 3 */
    idx_t r = n % 3;
    if (r == 0)              /* n = 3k   */
        return b;
    else if (r == 1)         /* n = 3k+1 */
        return (seq_t)5 * b;
    else                     /* r == 2, n = 3k+2 always 5 */
        return (seq_t)5;
}
