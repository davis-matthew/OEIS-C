/*
 * A025831  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^9)).
 * https://oeis.org/A025831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 16*n + 168 + 8*(n+6)*(1-(n mod 3)))/216)
 * Range:     u8 n <= 2, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t once */
    seq_t nseq = (seq_t)n;

    /* base = n^2 + 16*n + 168 */
    seq_t base = nseq * nseq + (seq_t)16 * nseq + (seq_t)168;

    /* compute n mod 3 */
    int mod3 = (int)(n % 3);

    /* add or subtract the extra term depending on mod3 */
    if (mod3 == 0) {
        base += (seq_t)8 * (nseq + (seq_t)6);
    } else if (mod3 == 2) {
        base -= (seq_t)8 * (nseq + (seq_t)6);
    }
    /* integer division gives floor for non‑negative numerator */
    return base / (seq_t)216;
}
