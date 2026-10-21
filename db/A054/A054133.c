/*
 * A054133  T(2n-1,n) where T is the array in A054126.
 * https://oeis.org/A054133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^(n-2) + Σ_{m=0}^{n-1} binomial(3*n-1, m) (n ≥ 2) a(1) =
 *            2 The binomial is evaluated multiplicatively to avoid overflow:
 *            C(N,0)=1, C(N, k)=C(N, k‑1) * (N-k+1)/k All arithmetic uses
 *            seq_t, which may be an arbitrary‑precision integer class
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)2;
    }

    /* Compute the binomial sum Σ_{m=0}^{n-1} C(3*n-1, m) */
    idx_t N = 3 * n - 1;          /* N = 3n-1 */
    seq_t sum  = 1;               /* C(N,0) */
    seq_t binom = 1;              /* current binomial coefficient C(N, m-1) */

    for (idx_t m = 1; m < n; ++m) {
        /* binom = binom * (N - m + 1) / m  */
        binom = binom * (N - m + 1) / m;
        sum += binom;
    }

    /* Compute 2^(n-2) */
    seq_t two_pow = 1;
    for (idx_t i = 0; i < n - 2; ++i) {
        two_pow *= 2;
    }

    return two_pow + sum;
}
