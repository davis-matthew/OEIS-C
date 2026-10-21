/*
 * A235963  n appears (n+1)/(1 + (n mod 2)) times.
 * https://oeis.org/A235963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = n appears (n + 1)/(1 + (n mod 2)) times
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t cumulative = 0;
    idx_t v = 0;
    /* iterate values until the cumulative count exceeds the requested index */
    while (cumulative <= n) {
        /* times(v) = v + 1   if v even
           times(v) = (v + 1)/2 if v odd   */
        idx_t times = (v & 1) ? (v + 1) / 2 : (v + 1);
        cumulative += times;
        if (cumulative > n)
            break;
        v++;
    }
    return (seq_t)v;
}
