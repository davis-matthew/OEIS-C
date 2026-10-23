/*
 * A107029  Diagonal sums of number triangle A107027.
 * https://oeis.org/A107029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..⌊n/2⌋} [ (n-2k-1)·C((n-2k)·k, k) -
 *            (n-2k-2)·∑_{j=0..k} C((n-2k)·k, j) ]
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t kmax = n >> 1;                     /* floor(n/2) */

    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t diff = n - (k << 1);          /* n - 2k (≥0) */

        int64_t a1 = (int64_t)diff - 1;     /* n-2k-1, possibly negative */
        int64_t a2 = (int64_t)diff - 2;     /* n-2k-2, possibly negative */

        idx_t N = diff * k;                 /* (n-2k)·k */

        /* Compute C(N,k) and S = Σ_{j=0..min(k,N)} C(N,j) */
        seq_t c   = 1;                      /* C(N,0)   */
        seq_t sum = 1;                      /* sum up to j=0 */
        idx_t limit = (N < k ? N : k);      /* min(k,N)  */

        for (idx_t j = 1; j <= limit; ++j) {
            unsigned __int128 tmp = (unsigned __int128)c * (unsigned __int128)(N - j + 1);
            tmp /= j;                      /* exact division */
            c = (seq_t)tmp;
            sum += c;
        }
        if (k > N) c = 0;                  /* C(N,k)=0 when k>N */

        /* Accumulate positive and negative contributions */
        seq_t pos = 0, neg = 0;
        if (a1 >= 0)                     /* (n-2k-1)·C(N,k) */
            pos += (seq_t)a1 * c;
        else
            neg += (seq_t)(-a1) * c;      /* -(|a1|)·C(N,k) */

        if (a2 >= 0)                     /* -(n-2k-2)·S -> subtract positive part */
            neg += (seq_t)a2 * sum;
        else
            pos += (seq_t)(-a2) * sum;    /* -(negative) -> add |a2|·S */

        seq_t term = pos - neg;           /* pos ≥ neg, final term is non‑negative */
        result += term;
    }

    return result;
}
