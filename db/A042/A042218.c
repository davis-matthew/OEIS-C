/*
 * A042218  Numerators of continued fraction convergents to sqrt(635).
 * https://oeis.org/A042218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 252*a(n-2) - a(n-4); a(0)=25, a(1)=126, a(2)=6325,
 *            a(3)=31751
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the known initial values */
    if (n == 0U) return (seq_t)25;
    if (n == 1U) return (seq_t)126;
    if (n == 2U) return (seq_t)6325;
    if (n == 3U) return (seq_t)31751;

    /* Sliding window of the last four computed values */
    seq_t values[4] = {25, 126, 6325, 31751};

    idx_t i;
    for (i = 4; i <= n; ++i)
    {
        /* a(i) = 252*a(i-2) - a(i-4) */
        seq_t new_val = (seq_t)252 * values[(i - 2) % 4] - values[(i - 4) % 4];
        values[i % 4] = new_val;
    }

    return values[n % 4];
}
