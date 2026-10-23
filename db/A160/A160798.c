/*
 * A160798  a(n) = A160797(n+2)/3.
 * https://oeis.org/A160798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A160797(n+2)/3
 * Range:     u8 n <= 61, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A160798 is constructed as follows:
 *  Row 1:           [1, 7]
 *  Row k+1:       Row k followed by each element of Row k multiplied by 3.
 *  The full sequence is the concatenation of all rows.
 *  This function computes a(n) for any n >= 1. */

seq_t a(idx_t n) {
    // Find row k such that the cumulative length up to row k is
    // 2^(k+1)-2 >= n, i.e. 2^(k+1) >= n+2
    idx_t m = n + 2;          // n+2 fits in idx_t
    idx_t pw = 1;             // 2^0
    idx_t l  = 0;             // current exponent
    while (pw < m) {          // find least l with 2^l >= m
        pw <<= 1;             // pw = 2^(l+1)
        l++;
    }
    idx_t k = l - 1;          // row index (k >= 1)

    idx_t row_len = (idx_t)1 << k;      // length of row k (2^k)
    // offset of n within row k (1-indexed)
    idx_t pos = n - (row_len - 2);

    seq_t multiplier = 1;     // accumulate factors of 3

    /* Walk back from row k to row 1, updating pos and multiplier */
    while (k > 1) {
        idx_t half = row_len >> 1;      // 2^(k-1)
        if (pos > half) {
            pos -= half;                // second half of the row
            multiplier *= (seq_t)3;     // multiply by 3
        }
        row_len = half;
        k--;
    }

    /* Base case: row 1 has values 1 (pos==1) and 7 (pos==2) */
    seq_t base = (pos == 1) ? (seq_t)1 : (seq_t)7;
    return base * multiplier;
}
