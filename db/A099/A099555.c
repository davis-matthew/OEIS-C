/*
 * A099555  Triangle, read by rows, where T(n,k) = (n-floor(k/2))^k for k =
 *          0..2*n - 1, with T(0,0) = 1.
 * https://oeis.org/A099555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 63, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A099555: Triangle read by rows
   T(n,k) = (n - floor(k/2))^k   for n >= 0, 0 <= k < 2*n  (T(0,0)=1)
   a(n) enumerates the triangle entries row by row, with offset 0.  */

/* Integer exponentiation by squaring for seq_t */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

/* a(n) for any non‑negative index n */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;          /* T(0,0) */

    /* Find the row r that contains the n‑th term.
       Rows: 0 → 1 term, r ≥ 1 → 2*r terms.
       The cumulative number of terms up to row r is r(r+1)+1.
       Find smallest r with r(r+1)+1 > n.  Linear search is fine for n ≤ 10 000. */
    idx_t r = 0;
    while ((r * (r + 1) + 1) <= n) ++r;

    /* Index of the first term of row r */
    idx_t start_of_row = (r == 0) ? 0 : ((r - 1) * r + 1);
    idx_t k = n - start_of_row;          /* 0 ≤ k < 2*r */

    /* Compute the base: n - floor(k/2) */
    idx_t base_val = r - (k >> 1);       /* floor(k/2) via shift */
    seq_t base = (seq_t)base_val;

    /* 0^0 is defined as 1 */
    if (base == (seq_t)0 && k == 0) return (seq_t)1;

    return pow_seq(base, k);
}
