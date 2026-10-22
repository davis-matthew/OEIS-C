/*
 * A001866  Number of connected graphs with n nodes and n edges.
 * https://oeis.org/A001866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!^2*n^(n-1)/2)*Sum_{k=2..n} n^(-k)/(n-k)!, rewritten as
 *            a(n) = (n!/2) * ( (n-1)! + Sum_{j=1..n-2} n^(j-1) * n!/j! ),
 *            with a(0) = a(1) = 0; the sum is evaluated by Horner's rule in n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) return 0;

    /* half = n!/2 = 3*4*...*n, fm1 = (n-1)! */
    seq_t half = 1;
    for (idx_t i = 3; i <= n; i++) half *= (seq_t)i;
    seq_t fm1 = 1;
    for (idx_t i = 2; i < n; i++) fm1 *= (seq_t)i;

    seq_t S = fm1;
    if (n >= 3) {
        /* P_j = n!/j! = (j+1)*(j+2)*...*n; H = Sum_{j=1..n-2} P_j * n^(j-1) */
        seq_t P = (seq_t)n * (seq_t)(n - 1);   /* P_{n-2} */
        seq_t H = P;
        for (idx_t j = n - 2; j >= 2; j--) {
            P *= (seq_t)j;                       /* P_{j-1} */
            H = H * (seq_t)n + P;
        }
        S += H;
    }
    return half * S;
}
