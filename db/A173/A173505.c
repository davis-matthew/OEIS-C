/*
 * A173505  Triangle T(n, k, q) = c(n, q)/(c(k, q)*c(n-k, q)) where c(n,q) =
 *          Product_{j=1..n} (q^j -1)^(n-j) and q = 4, read by rows.
 * https://oeis.org/A173505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) = T(r,k,4) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r(r+1)/2, and T(n,k,4) = c(n,4)/(c(k,4)*c(n-k,4)) with c(m,4) =
 *            Π_{j=1..m} (4^j - 1)^(m-j) (OEIS A173505)
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 22, u64 n <= 29, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_local(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* --- Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 --- */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* Border cases: first and last entry of each row */
    if (k == 0 || k == r)
        return 1;

    idx_t a_k = k;
    idx_t b_k = r - k;

    seq_t result = 1;
    seq_t power = 4;  /* holds 4^j during the loop */

    for (idx_t j = 1; j <= r; ++j) {
        seq_t base = power - 1;  /* 4^j - 1 */
        idx_t exp;

        if (j <= a_k && j <= b_k)
            exp = j;
        else if (j <= a_k)      /* a_k < b_k */
            exp = b_k;
        else if (j <= b_k)      /* b_k < a_k */
            exp = a_k;
        else
            exp = r - j;

        if (exp != 0) {
            seq_t p = pow_seq_local(base, exp);
            result = result * p;
        }

        /* prepare 4^{j+1} for the next iteration */
        if (j < r)
            power = power * 4;
    }

    return result;
}
