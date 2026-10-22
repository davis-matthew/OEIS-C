/*
 * A001471  Number of degree-n permutations of order exactly 3.
 * https://oeis.org/A001471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = A001470(n) - 1, where A001470(n) = A001470(n-1) +
 *            (n-1)*(n-2)*A001470(n-3), A001470(0) = A001470(1) = A001470(2) =
 *            1; equivalently a(n) = a(n-1) + (1 + a(n-3))*(n-1)*(n-2), a(0) =
 *            a(1) = a(2) = 0
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t b0, b1, b2, bn;
    idx_t k;
    if (n < 3) return 0;
    b0 = 1; /* A001470(k-3) */
    b1 = 1; /* A001470(k-2) */
    b2 = 1; /* A001470(k-1) */
    for (k = 3; k <= n; k++) {
        bn = b2 + ((seq_t)k - 1) * ((seq_t)k - 2) * b0;
        b0 = b1;
        b1 = b2;
        b2 = bn;
    }
    return b2 - 1;
}
