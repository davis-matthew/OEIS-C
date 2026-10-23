/*
 * A133188  Natural numbers listed in three columns: if A004526(n-1) = 0 then
 *          row n lists A004526(n-1), A004526(n), 1, otherwise row n lists 1,
 *          A004526(n), A004526(n-1).
 * https://oeis.org/A133188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   flatten rows of length 3 derived from A004526. For row r, let p
 *            = floor((r-1)/2), q = floor(r/2). If p==0 the row is [p,q,1],
 *            otherwise [1,q,p]. The flat index n is 1‑based; a(n) is the
 *            corresponding element
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t row = (n - 1) / 3 + 1;
    idx_t pos = (n - 1) % 3;

    seq_t prev = ((seq_t)(row - 1)) / 2;   /* A004526(row-1) */
    seq_t curr = ((seq_t)row) / 2;         /* A004526(row)   */

    if (prev == 0) {
        if (pos == 0) return prev;
        if (pos == 1) return curr;
        return (seq_t)1;
    } else {
        if (pos == 0) return (seq_t)1;
        if (pos == 1) return curr;
        return prev;
    }
}
