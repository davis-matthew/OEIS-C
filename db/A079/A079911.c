/*
 * A079911  Solution to the Dancing School Problem with 6 girls and n+6 boys:
 *          f(6,n).
 * https://oeis.org/A079911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 - 9*n^5 + 60*n^4 - 225*n^3 + 555*n^2 - 774*n + 484
 *            (for n > 3) Initial values: a(0) = 1, a(1) = 7, a(2) = 79, a(3)
 *            = 478
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)79;
    if (n == 3) return (seq_t)478;

    seq_t sn  = (seq_t)n;          /* n */
    seq_t n2  = sn * sn;           /* n^2 */
    seq_t n3  = n2 * sn;           /* n^3 */
    seq_t n4  = n3 * sn;           /* n^4 */
    seq_t n5  = n4 * sn;           /* n^5 */
    seq_t n6  = n5 * sn;           /* n^6 */

    /* Compute the polynomial using unsigned arithmetic (safe for n > 3). */
    seq_t result = n6
                   - (seq_t)9   * n5
                   + (seq_t)60  * n4
                   - (seq_t)225 * n3
                   + (seq_t)555 * n2
                   - (seq_t)774 * sn
                   + (seq_t)484;

    return result;
}
