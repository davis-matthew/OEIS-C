/*
 * A174389  Triangle T(n, k) = c(n, q)/c(k, q) if k <= floor(n/2), otherwise
 *          c(n, q)/c(n-k, q), where c(n, q) = Product_{j=1..n} (1 - q^j) and
 *          q = 4, read by rows.
 * https://oeis.org/A174389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = product_{j=max(k,n-k)+1}^{r} (1-4^j) where r is the row
 *            index such that (r+1)(r+2)/2 > n >= r(r+1)/2 and k = n -
 *            r(r+1)/2. The triangle T(n,k) satisfies
 *            T(n,k)=c(n,4)/c(max(k,n-k),4)
 * Range:     i8 n <= 10, i16 n <= 16, i32 n <= 30, i64 n <= 58, i128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t compute_T(idx_t r, idx_t k)
{
    idx_t m = (k > r - k) ? k : (r - k);
    seq_t pow4 = 1;
    seq_t res = 1;
    for (idx_t j = 1; j <= r; ++j) {
        pow4 = pow4 * 4;
        if (j > m) {
            res = res * ((seq_t)1 - pow4);
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t prev = r * (r + 1) / 2;
    idx_t k = n - prev;
    return compute_T(r, k);
}
