/*
 * A001864  Total height of rooted trees with n labeled nodes.
 * https://oeis.org/A001864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n! * Sum_{k=0..n-2} n^k/k!, evaluated by Horner's scheme:
 *            U_0 = 1, U_m = m*U_(m-1) + n^m for m = 1..n-2, a(n) =
 *            U_(n-2)*(n-1)*n; a(1) = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) return 0;
    seq_t N = (seq_t)n;
    seq_t U = 1;
    seq_t pw = 1;
    for (idx_t m = 1; m + 2 <= n; m++) {
        pw *= N;
        U = U * (seq_t)m + pw;
    }
    return U * (N - 1) * N;
}
