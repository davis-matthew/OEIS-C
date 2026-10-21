/*
 * A095200  Greatest multiple of n of the form (n-1) + (n-2) + ... + (n-k), or
 *          0 if no such number exists.
 * https://oeis.org/A095200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ k*(2*n - k - 1)/2 : 1 ≤ k < n, n divides k*(2*n - k
 *            - 1)/2 }, 0 if none
 * Range:     u8 n <= 16, u16 n <= 289, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                     /* a(1) = 0 as defined */

    seq_t N   = n;                    /* convert idx_t to seq_t */
    seq_t best = 0;
    const seq_t two = 2;

    for (idx_t k = 1; k < n; ++k) {
        seq_t K = k;                  /* convert k to seq_t */
        seq_t t = two * N - K - 1;    /* 2*n - k - 1 */
        seq_t prod = K * t;
        seq_t S = prod / 2;           /* integer division – guaranteed even */

        if ((S % N) == 0 && S > best)
            best = S;
    }
    return best;
}
