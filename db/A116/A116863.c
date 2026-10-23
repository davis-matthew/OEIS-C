/*
 * A116863  Array used to find the eigenvalues of the quadratic Casimir
 *          operator for the Lie algebras A_n = su(n+1), n>=1.
 * https://oeis.org/A116863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the n‑th coefficient in the concatenation of rows of
 *            coefficients for the quadratic Casimir polynomial of the Lie
 *            algebra A_r = su(r+1). Row r contains the coefficients of
 *            (n+1)*rowvec(n,2+a)*C⁻¹(n)*colvec(n,a) in the order: 11,12,…,1r,
 *            22,23,…,2r, …, rr, followed by the linear coefficien...
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t idx = n;          /* make sure we work with unsigned indices   */
    if (idx == 0) return 0; /* undefined for offset 0, just defensive    */

    idx_t r = 1;            /* current row index (1‑based)              */
    idx_t sum = 0;          /* cumulative count of terms up to row r‑1  */
    idx_t len = (r * (r + 3)) / 2;   /* length of row r                  */

    /* Find the row that contains the index n   */
    while ((idx - sum) > len) {
        sum += len;
        r++;
        len = (r * (r + 3)) / 2;
    }

    /* Position inside the chosen row (1‑based)  */
    idx_t pos = idx - sum;

    /* Number of quadratic terms in this row   */
    idx_t t = (r * (r + 1)) / 2;

    seq_t result = 0;

    if (pos <= t) { /* Quadratic coefficient   */
        idx_t remaining = pos;
        idx_t i = 1;
        /* Find the i such that the remaining offset fits in the block for this i */
        while (remaining > (r - i + 1)) {
            remaining -= (r - i + 1);
            i++;
        }
        idx_t j = i + remaining - 1;   /* j >= i, 1‑based */

        if (i == j) {
            /* diagonal coefficient: i*(r+1-i) */
            result = (seq_t)i * ((seq_t)(r + 1) - (seq_t)i);
        } else {
            /* off‑diagonal: 2*i*(r+1-j) */
            result = (seq_t)2 * (seq_t)i * (((seq_t)(r + 1) - (seq_t)j));
        }
    } else { /* Linear coefficient   */
        idx_t k = pos - t;                /* 1‑based linear index   */
        /* linear coeff: (r+1)*k*(r+1-k) */
        result = (seq_t)(r + 1) * (seq_t)k *
                 (((seq_t)(r + 1) - (seq_t)k));
    }

    return result;
}
