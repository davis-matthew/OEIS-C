/*
 * A169653  Triangle T(n,k) = A008297(n,k) + A008297(n,n-k+1), read by rows.
 * https://oeis.org/A169653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n‑th element read by rows of the triangle
 *            T(n,k)=A008297(n,k)+A008297(n,n-k+1). T(n,k)=(-1)^n * (n!/k!) *
 *            binomial(n-1,k-1) + (-1)^n * (n!/(n-k+1)!) * binomial(n-1,n-k)
 * Range:     i8 n <= 11, i16 n <= 28, i32 n <= 67, i64 n <= 191,
 *            i128 n <= 530, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t compute_t(idx_t n, idx_t k) {
    /* t(n,k) = (-1)^n * (n!/k!) * C(n-1,k-1) */
    seq_t sign = ((n & 1) == 0) ? 1 : -1;  /* (-1)^n  */
    seq_t res  = 1;

    /* product (n!/k!)  ->  ∏_{i=k+1}^{n} i  */
    for (idx_t i = k + 1; i <= n; ++i) {
        res *= (seq_t)i;
    }

    /* multiply by binomial(n-1,k-1)  -> ∏_{i=1}^{k-1} (n-i)/i */
    for (idx_t i = 1; i < k; ++i) {
        res = res * (seq_t)(n - i) / (seq_t)i;
    }

    if (sign == -1) res = -res;
    return res;
}

static seq_t T_triangle(idx_t n, idx_t k) {
    seq_t t1 = compute_t(n, k);
    idx_t k2 = n - k + 1;
    seq_t t2 = compute_t(n, k2);
    return t1 + t2;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety for n=0, unused in the sequence */

    /* locate the row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    seq_t rsum = 1;                /* r*(r+1)/2 for r=1 */
    while (rsum < n) {
        ++r;
        rsum = (seq_t)r * ((seq_t)r + 1) / 2;
    }

    /* position k within that row : 1 ≤ k ≤ r */
    seq_t prevSum = (seq_t)(r - 1) * (seq_t)r / 2;   /* sum of previous rows */
    idx_t k = (idx_t)(n - prevSum);

    return T_triangle(r, k);
}
