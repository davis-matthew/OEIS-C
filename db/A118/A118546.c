/*
 * A118546  A symmetrical triangle of coefficient weights: A117662 :f(n)=n*(n
 *          - 1)*(n - 2)*(n + 3)/12; t(n,m)=f(n - m + 1)*f(m + 1).
 * https://oeis.org/A118546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the sequence obtained by listing the non‑zero entries *
 *            of the triangle t(n,m) = f(n-m+1) * f(m+1) * where f(k) =
 *            k*(k-1)*(k-2)*(k+3) / 12. * The triangle is read row‑wise
 *            starting from row 4 (rows 1–3 are all zeros)
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t f(idx_t n)
{
    seq_t a = (seq_t)n;
    seq_t b = a - 1;
    seq_t c = a - 2;
    seq_t d = a + 3;
    seq_t prod = a * b;
    prod = prod * c;
    prod = prod * d;
    return prod / 12;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1, return 0 */
    /* Find smallest x such that x*(x+1)/2 >= n  (x >= 1) */
    idx_t x = 0;
    while (x * (x + 1) / 2 < n) ++x;
    /* Compute row r = x + 3 (since row 4 corresponds to x = 1) */
    idx_t r = x + 3;
    /* Number of values before this row */
    idx_t prev = (x - 1) * x / 2;
    /* Position within the row (1‑based) */
    idx_t k = n - prev;
    /* Column index m in the triangle */
    idx_t m = k + 1;
    /* Left factor index: left = r - k (equals n - m + 1) */
    idx_t left = r - k;
    /* Compute the term: f(left) * f(m + 1) */
    return f(left) * f(m + 1);
}
