/*
 * A092921  Array F(k, n) read by descending antidiagonals: k-generalized
 *          Fibonacci numbers in row k >= 1, starting (0, 1, 1, ...), for
 *          column n >= 0.
 * https://oeis.org/A092921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   read the array F(k,n) by antidiagonals. F(k,n) =
 *            F(k,n-1)+...+F(k,n-k), F(k,1)=1, F(k,n)=0 for n<=0
 * Range:     u8 n <= 92, u16 n <= 254, u32 n <= 782, u64 n <= 2631,
 *            u128 n <= 9457, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For each n>=0, a(n) is the value on the (k,n) cell located on the
   antidiagonal with k + n = d, where d is the smallest integer with
   d*(d+1)/2 > n. The k inside each antidiagonal is traversed in
   ascending order (k=1,2,...,d). */

static inline idx_t triangular(idx_t d) { return d * (d + 1) / 2; }

seq_t a(idx_t n)
{
    /* Find the diagonal d that contains the index n. */
    idx_t d = 0;
    while (triangular(d) <= n) ++d;

    /* Number of elements before this diagonal. */
    idx_t prev = triangular(d - 1);

    /* Position within the diagonal (0‑based). */
    idx_t p = n - prev;        /* 0 <= p < d */

    /* Row (k) and column (row index for F) inside the array. */
    idx_t k = 1 + p;           /* 1 <= k <= d */
    idx_t m = d - 1 - p;       /* 0 <= m <= d-1,   ncol for F(k,m) */

    /* Base cases for the generalized Fibonacci sequence. */
    if (m == 0) return (seq_t)0;
    if (m == 1) return (seq_t)1;

    /* Compute F(k,m) iteratively.  d <= about 150 for n <= 10^4,
       so a fixed static buffer is safe for all builds. */
    static seq_t f[200];   /* 200 > max d needed by the problem limits */

    f[0] = (seq_t)0;
    f[1] = (seq_t)1;

    for (idx_t i = 2; i <= m; ++i) {
        seq_t sum = (seq_t)0;
        idx_t tmax = k;
        if (tmax > i) tmax = i;      /* only existing previous terms */
        for (idx_t t = 1; t <= tmax; ++t) {
            sum += f[i - t];
        }
        f[i] = sum;
    }

    return f[m];
}
