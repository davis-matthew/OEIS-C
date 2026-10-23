/*
 * A037011  Baum-Sweet cubic sequence.
 * https://oeis.org/A037011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (∑_{k=0}^{n-1} binom(n-1+k, n-1-k) * binom(n-1,k)) mod 2
 *            where binom(x,y) is the binomial coefficient. The sum reduces to
 *            counting k such that both binomials are odd. By Lucas' theorem a
 *            binomial is odd iff (y & (x-y)) == 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* offset starts at 1, so a(1) corresponds to n=1 */
    if (n == 0)
        return (seq_t)0;   /* undefined, but return 0 for safety */
    uint64_t N = (uint64_t)(n - 1);
    seq_t parity = 0;       /* keep only the parity (0 or 1) */

    for (uint64_t k = 0; k <= N; ++k) {
        /* binom(N, k) is odd iff (k & (N-k)) == 0 */
        if ((k & (N - k)) != 0)
            continue;

        /* binom(N+k, N-k) is odd iff ((N-k) & (N+k-(N-k))) == 0
                                   i.e. (N-k) & (2*k) == 0 */
        uint64_t b1 = N - k;           /* N-k */
        if ((b1 & (k << 1)) != 0)     /* (2*k) */
            continue;

        /* both binomials are odd -> contributes 1 mod 2 */
        parity ^= 1;
    }
    return parity;
}
