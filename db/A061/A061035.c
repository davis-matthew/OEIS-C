/*
 * A061035  Triangle T(m,n) = numerator of 1/m^2 - 1/n^2, n >= 1,
 *          m=n,n-1,n-2,...,1.
 * https://oeis.org/A061035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, nice, frac
 * Formula:   a(n) = numerator of (1/m^2 - 1/n^2) in lowest terms, where the
 *            flattened triangle rows are listed reversed (row m contains the
 *            values for n=m,m−1,…,1). The term a(n) is defined for n ≥ 1
 * Range:     u8 n <= 11, u16 n <= 768, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* locate the row m such that 1+2+…+m ≥ n */
    idx_t m = 1;
    while ((seq_t)m * (m + 1) / 2 < n) {
        ++m;
    }

    /* number of terms before this row */
    idx_t prev = m * (m - 1) / 2;

    /* 0‑based position inside the row */
    idx_t pos = n - prev - 1;

    /* corresponding n value (row reversed) */
    idx_t n_val = m - pos;

    /* squares */
    seq_t mm  = (seq_t)m;
    seq_t nn  = (seq_t)n_val;
    seq_t mm2 = mm * mm;
    seq_t nn2 = nn * nn;

    /* absolute difference of squares */
    seq_t diff = (mm2 > nn2) ? (mm2 - nn2) : (nn2 - mm2);
    if (diff == 0) return 0;

    /* denominator of the fraction */
    seq_t denom = mm2 * nn2;

    /* Euclidean GCD for seq_t (works for uint8_t…__int128) */
    seq_t a_val = diff, b_val = denom;
    while (b_val != 0) {
        seq_t r = a_val % b_val;
        a_val = b_val;
        b_val = r;
    }
    seq_t g = a_val;

    /* reduced numerator */
    return diff / g;
}
