/*
 * A123964  Triangle, T(n, k) = k^6 - n^6 - 5*(n*k)^2*(n^2 - k^2) +
 *          4*n*k*((n*k)^4 - 1), read by rows.
 * https://oeis.org/A123964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = T(r,k) where T(r,k) = k^6 - r^6 - 5*(r*k)^2*(r^2 - k^2) +
 *            4*r*k*((r*k)^4 - 1), read by rows. a(0) = 0, a(1) = -1, a(2) =
 *            0, a(3) = -64, a(4) = -3, a(5) = 4080,
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 42, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Start index of this row */
    idx_t start = r * (r + 1) / 2;

    /* Column index within the row */
    idx_t k = n - start;

    /* Convert to seq_t for arithmetic */
    seq_t r_seq = (seq_t)r;
    seq_t k_seq = (seq_t)k;

    /* Basic powers */
    seq_t r2 = r_seq * r_seq;        /* r^2 */
    seq_t r4 = r2 * r2;              /* r^4 */
    seq_t r6 = r4 * r2;              /* r^6 */

    seq_t k2 = k_seq * k_seq;        /* k^2 */
    seq_t k4 = k2 * k2;              /* k^4 */
    seq_t k6 = k4 * k2;              /* k^6 */

    /* (r*k) and its powers */
    seq_t rk = r_seq * k_seq;        /* r*k */
    seq_t rk2 = rk * rk;             /* (r*k)^2 */
    seq_t rk4 = rk2 * rk2;           /* (r*k)^4 */

    /* Differences */
    seq_t r2_minus_k2 = r2 - k2;

    /* Compute each term of the polynomial */
    seq_t term1 = k6;                                /* k^6 */
    seq_t term2 = r6;                                /* r^6 */
    seq_t term3 = 5 * rk2 * r2_minus_k2;             /* 5*(r*k)^2*(r^2 - k^2) */
    seq_t term4 = 4 * r_seq * k_seq * (rk4 - 1);      /* 4*r*k*((r*k)^4 - 1) */

    /* Combine all parts to obtain the triangle entry */
    seq_t result = term1 - term2 - term3 + term4;
    return result;
}
