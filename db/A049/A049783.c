/*
 * A049783  Triangular array T read by rows: T(n,k) = Sum_{j=1..k} (n mod
 *          floor(k/j)) for n, k >= 1.
 * https://oeis.org/A049783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) entry of the triangular array defined by
 *            T(n,k) = Σ_{j=1..k} (n mod ⌊k/j⌋). The n‑th element is unpacked
 *            as: r = smallest integer with r(r+1)/2 ≥ n (row index) k = n –
 *            r(r-1)/2 (column index) a(n) = T(r,k)
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find row r */
    idx_t r = 1;
    while ((r * (r + 1) / 2) < n)
        ++r;

    /* column index k */
    idx_t k = n - (r - 1) * r / 2;

    /* compute T(r,k) */
    seq_t sum = 0;
    for (idx_t j = 1; j <= k; ++j) {
        seq_t d = (seq_t)(k / j);      /* floor(k/j) */
        sum += (seq_t)(r % d);          /* r mod d */
    }
    return sum;
}
