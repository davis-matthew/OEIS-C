/*
 * A220198  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical or diagonal neighbors in a
 *          random, but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..3 n X 2 array.
 * https://oeis.org/A220198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^{9} N_i * C(n - i - 1 + 8, 8) where N = {4, -26,
 *            92, -215, 350, -386, 277, -119, 29, -4}
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 55, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const int N[10] = {
        4, -26, 92, -215, 350,
        -386, 277, -119, 29, -4
    };
    __int128 sum = 0;
    long long nn = (long long)n;          /* safe conversion: n ≤ 10000 in tests */

    for (int i = 0; i < 10; ++i) {
        long long m = nn - (i + 1) + 8;   /* m = n - i - 1 + 8 */
        if (m < 8) continue;              /* binomial is zero for m < 8 */

        __int128 binom = 1;
        /* compute C(m,8) multiplicatively */
        for (int j = 1; j <= 8; ++j) {
            binom = binom * (__int128)(m - 8 + j) / j;
        }

        __int128 term = binom * (__int128)N[i];
        sum += term;                      /* N[i] may be negative */
    }

    return (seq_t)sum;
}
