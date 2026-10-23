/*
 * A143037  Triangle read by rows, A000012 * A127773 * A000012. A000012 is an
 *          infinite lower triangular matrix with all 1's, A127773 = (1; 0,3;
 *          0,0,6; 0,0,0,10; ...).
 * https://oeis.org/A143037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,c) where T(r,c) = c*(c^2 - 3*c*r - 3*c + 3*r^2 + 6*r
 *            + 2) / 6 (row r, column c are obtained from the n‑th position in
 *            the triangle)
 * Range:     u8 n <= 17, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find the smallest r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    idx_t triangular = 1;          /* triangular number T(r)=r*(r+1)/2 for r=1 */
    while (triangular < n) {
        ++r;
        triangular += r;           /* next triangular number */
    }

    /* column index k within row r */
    idx_t k = n - (triangular - r);   /* offset inside the row */

    /* compute T(r,k) using the closed form */
    seq_t n_seq = (seq_t)r;
    seq_t k_seq = (seq_t)k;

    /* avoid unsigned underflow: compute positive part minus negative part */
    seq_t pos = k_seq * k_seq + 3 * n_seq * n_seq + 6 * n_seq + 2;
    seq_t neg = 3 * k_seq * n_seq + 3 * k_seq;
    seq_t diff = pos - neg;              /* guaranteed non‑negative for k <= r */

    seq_t numerator = k_seq * diff;
    seq_t value = numerator / 6;         /* division is exact */

    return value;
}
