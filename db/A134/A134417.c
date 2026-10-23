/*
 * A134417  Triangle read by rows: T(n,k) = binomial(n,k)*A133632(k + 1).
 * https://oeis.org/A134417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry in row n/∗ row count ∗ of the triangle defined
 *            by T(n,k) = binomial(n,k) * A133632(k+1). The flattening is
 *            row‑by‑row: 1; 1,4; 1,8,5;
 * Range:     u8 n <= 23, u16 n <= 72, u32 n <= 222, u64 n <= 806,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: compute 5^e in seq_t */
static seq_t pow5(idx_t e) {
    seq_t res = 1;
    for (idx_t i = 0; i < e; ++i)
        res *= 5;
    return res;
}

/* Helper: the A133632 sequence */
static seq_t a133632(idx_t n) {          /* n >= 1 */
    if ((n & 1) == 1) {                 /* n odd */
        return pow5((n - 1) / 2);
    } else {                            /* n even */
        seq_t v = pow5((n - 2) / 2);
        return v * 4;
    }
}

/* Helper: compute binomial(n,k) using multiplicative formula */
static seq_t binomial_seq(idx_t n, idx_t k) {
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (n - k + i);
        res /= i;
    }
    return res;
}

/* Main function: a(n) for OEIS sequence A134417 */
seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = 1;
    while (high * (high + 1) / 2 <= n)
        high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) / 2;
        if (mid * (mid + 1) / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;                 /* row index */
    idx_t before = r * (r + 1) / 2;    /* terms before this row */
    idx_t k = n - before;              /* column in the row */

    seq_t b = binomial_seq(r, k);      /* binomial(r,k) */
    seq_t a133 = a133632(k + 1);       /* A133632(k+1) */

    return b * a133;
}
