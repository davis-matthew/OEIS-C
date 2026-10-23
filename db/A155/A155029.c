/*
 * A155029  Complement to A051731 with the identity matrix A023531 included.
 * https://oeis.org/A155029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The sequence is obtained by reading the 0/1 triangle * defined
 *            by * T(n,1) = 0 for all n>1 (T(1,1)=1) * T(n,n) = 1 * T(n,k) = 0
 *            if n mod k == 0 (for 1<k<n) * T(n,k) = 1 otherwise * in
 *            row‑major order. Thus a(1)=T(1,1)=1, a(2)=T(2,1)=0,
 *            a(3)=T(2,2)=1,
 * Range:     u8 n <= 465, u16 n <= 465, u32 n <= 465, u64 n <= 465,
 *            u128 n <= 465, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t row = 0;
    idx_t cum = 0;
    /* Find the row that contains the n‑th element */
    while (cum < n) {
        ++row;
        cum = row * (row + 1) / 2;
    }
    /* Compute column within that row (1‑indexed) */
    idx_t prev = (row - 1) * row / 2;
    idx_t col = n - prev;

    /* Special diagonal and first column handling */
    if (row == 1)
        return (seq_t)1;           /* T(1,1)=1 */
    if (col == 1)
        return (seq_t)0;           /* T(n,1)=0 for n>1 */
    if (col == row)
        return (seq_t)1;           /* diagonal */

    /* General rule */
    if (row % col == 0)
        return (seq_t)0;           /* divisible */
    return (seq_t)1;               /* otherwise */
}
