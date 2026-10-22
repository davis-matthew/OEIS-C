/*
 * A005262  a(n) = floor((7*2^(n+1)-9*n-10)/3).
 * https://oeis.org/A005262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*n - 3 for even n, 2*a(n-1) + 3*n - 2 for odd
 *            n, a(0) = 1 (equivalent to a(n) = floor((7*2^(n+1)-9*n-10)/3))
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        v = 2 * v + 3 * (seq_t)k - ((k & 1) ? 2 : 3);
    }
    return v;
}
