/*
 * A167051  Start at 1, then add the first term (which is one here) plus 1 for
 *          the second term; then add the second term plus 2 for the third
 *          term; then add the third term to the sum of the first and second
 *          term; this gives the fourth term. Restart the sequence by adding 1
 *          to the fourth term, etc. (From a si...
 * https://oeis.org/A167051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2, a(3) = 4. For n > 3: if n mod 3 == 1: a(n) =
 *            a(n-1) + a(n-2) + a(n-3); if n mod 3 == 2: a(n) = a(n-1) + 1; if
 *            n mod 3 == 0: a(n) = a(n-1) + 2
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 60, u64 n <= 120,
 *            u128 n <= 240, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    seq_t a1 = (seq_t)1;   /* a(n-3)   */
    seq_t a2 = (seq_t)2;   /* a(n-2)   */
    seq_t a3 = (seq_t)4;   /* a(n-1)   */
    seq_t an = a3;         /* current term */

    for (idx_t i = 4; i <= n; ++i)
    {
        if (i % 3 == 1) {
            an = a3 + a2 + a1;
        } else if (i % 3 == 2) {
            an = a3 + (seq_t)1;
        } else { /* i % 3 == 0 */
            an = a3 + (seq_t)2;
        }
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
