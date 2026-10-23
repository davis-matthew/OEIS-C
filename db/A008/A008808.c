/*
 * A008808  Expansion of (1+x^7)/((1-x^2)^2*(1-x^7)).
 * https://oeis.org/A008808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{m=0}^{⌊n/7⌋} [ (n-7m) even ? (1 + (m>0)) * ((n-7m)/2
 *            + 1) : 0 ]
 * Range:     u8 n <= 82, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t limit = n / 7;
    for (idx_t m = 0; m <= limit; ++m) {
        idx_t n_minus = n - 7 * m;
        if ((n_minus & 1) != 0) continue;          /* odd, skip */
        idx_t k = n_minus / 2;                    /* k >= 0 */
        seq_t weight = (seq_t)(k + 1);
        if (m > 0) weight = weight * 2;
        result += weight;
    }
    return result;
}
