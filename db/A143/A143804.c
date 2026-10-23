/*
 * A143804  Triangle read by rows, thrice the Connell numbers (A001614) - 2.
 * https://oeis.org/A143804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3*A001614(n) - 2
 * Range:     u8 n <= 47, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the n‑th term of the row‑wise flattened
   triangle:  a(n) = 3*(r-1)^2 + 1 + 6*(n - r*(r-1)/2 - 1),
   where r is the row containing the n‑th element. */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* convert index to seq_t */
    seq_t r = 1;                 /* current row (1‑based)       */
    seq_t tri = 1;               /* triangle number r*(r+1)/2   */

    /* find minimal r such that tri >= N */
    while (tri < N) {
        ++r;
        tri = r * (r + (seq_t)1) / (seq_t)2;
    }

    seq_t p = r - (seq_t)1;                     /* r-1                 */
    seq_t offset = p * (p + (seq_t)1) / (seq_t)2; /* terms before row r */
    seq_t k = N - offset - (seq_t)1;             /* column index 0‑based  */

    seq_t value = (seq_t)3 * p * p           /* left border = 3*(r-1)^2 + 1   */
              + (seq_t)1
              + (seq_t)6 * k;                 /* + 6*k to move right in the row */

    return value;
}
