/*
 * A055607  a(2n+1) = n^2 - 1 + A002620(n), a(2n) = a(2n-1) + n.
 * https://oeis.org/A055607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n+1) = n^2 - 1 + floor((n+2)^2 / 4) a(2n) = a(2n-1) + 3n/2 -
 *            1 offset: 1
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety for n=0, unused */
    if ((n & 1) == 1) {              /* odd index: a(2n+1) */
        seq_t k = (seq_t)((n - 1) / 2);
        seq_t term1 = k * k - (seq_t)1;
        seq_t term2 = ((k + (seq_t)2) * (k + (seq_t)2)) / (seq_t)4;
        return term1 + term2;
    } else {                         /* even index: a(2n)   */
        seq_t k = (seq_t)(n / 2);
        seq_t m = k - (seq_t)1;      /* m = n/2 - 1  -> index for a(2n-1) */
        seq_t a_odd = m * m - (seq_t)1
                       + ((m + (seq_t)2) * (m + (seq_t)2)) / (seq_t)4;
        seq_t diff = (k * (seq_t)3) / (seq_t)2 - (seq_t)1;
        return a_odd + diff;
    }
}
