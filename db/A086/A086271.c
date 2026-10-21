/*
 * A086271  Rectangular array T(n,k) of polygonal numbers, by descending
 *          antidiagonals.
 * https://oeis.org/A086271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = k*C(n,2)+n where (n,k) are the row and column of the n-th
 *            entry when the two‑dimensional array T(n,k) = k*binom(n,2)+n is
 *            read by antidiagonals with n increasing (i.e., n=1,2,… on each
 *            diagonal). This matches the OEIS ordering used in A086271
 * Range:     u8 n <= 113, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so a(1) is the first entry. */
    if (n < 1)
        return 0;

    /* Find the diagonal sum d = n + k for which the n‑th term
       lies.  Each diagonal d has (d-1) elements, and the number
       of elements up to and including diagonal d is
       S(d) = d*(d-1)/2.  Find the smallest d with S(d) >= n. */
    idx_t d = 2;
    idx_t triangular = d * (d - 1) / 2;   /* S(d) */
    while (triangular < n) {
        ++d;
        triangular = d * (d - 1) / 2;
    }

    /* Number of elements before this diagonal. */
    idx_t before = (d - 2) * (d - 1) / 2;  /* S(d-1) */

    /* Offset within the diagonal (0‑based). */
    idx_t offset = n - before - 1;

    /* Coordinates on the array: row nIdx increases from 1
       up to d-1, while column kIdx decreases accordingly. */
    idx_t nIdx = offset + 1;          /* row index */
    idx_t kIdx = d - nIdx;            /* column index */

    /* Compute T(nIdx, kIdx) = kIdx * binom(nIdx, 2) + nIdx. */
    seq_t nSeq = (seq_t)nIdx;
    seq_t kSeq = (seq_t)kIdx;
    seq_t c = nSeq * (nSeq - 1) / 2;  /* binom(nIdx, 2) */

    return kSeq * c + nSeq;
}
