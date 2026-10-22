/*
 * A003469  Number of minimal covers of an (n + 1)-set by a collection of n
 *          nonempty subsets, a(n) = A035348(n,n-1).
 * https://oeis.org/A003469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)*2^n - (n + 1)*(n + 2)/2
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p = 1;
    idx_t i;
    idx_t h1, h2;
    for (i = 0; i < n; i++)
        p = p * 2;
    /* halve the even factor of (n+1)(n+2) in idx_t before converting */
    if (n % 2 == 1) {
        h1 = (n + 1) / 2;
        h2 = n + 2;
    } else {
        h1 = n + 1;
        h2 = (n + 2) / 2;
    }
    return ((seq_t)n + 1) * p - (seq_t)h1 * (seq_t)h2;
}
