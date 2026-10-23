/*
 * A047780  Number of inequivalent ways to color faces of a cube using at most
 *          n colors.
 * https://oeis.org/A047780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 8*C(n, 2)+30*C(n, 3)+68*C(n, 4)+75*C(n, 5)+30*C(n, 6)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 68, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (seq_t)(n - k + i) / i; /* integer division is exact */
    }
    return r;
}

seq_t a(idx_t n) {
    seq_t result = n;                    /* term for k = 1 */

    result += 8 * binom(n, 2);
    result += 30 * binom(n, 3);
    result += 68 * binom(n, 4);
    result += 75 * binom(n, 5);
    result += 30 * binom(n, 6);

    return result;
}
