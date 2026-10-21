/*
 * A219736  The number of 2 X 2 matrices with all eigenvalues real and whose
 *          entries are integers with absolute value at most n.
 * https://oeis.org/A219736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)^4 - A207259(n) Here we compute the number of 2×2
 *            integer matrices with entries in [-n,n] having a non‑real
 *            eigenvalue and subtract from the total number of matrices
 *            (2*n+1)^4
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 140, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int64_t floor_div(int64_t a, int64_t b) {
    /* floor division for signed integers, b>0 */
    if (a >= 0) return a / b;
    return - ((-a + b - 1) / b);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1, but safe */

    int64_t N = (int64_t)n;         /* integer form of n */
    /* total number of 2x2 matrices with entries in [-n,n] */
    __int128 one = 2 * N + 1;
    __int128 total128 = one * one * one * one;
    seq_t total = (seq_t)total128;

    int64_t complexCount = 0;
    int64_t limit = 2 * N;          /* |s| <= 2n */

    for (int64_t s = -limit; s <= limit; ++s) {
        int64_t s_abs = (s >= 0) ? s : -s;
        int64_t weight = 2 * N - s_abs + 1;   /* number of (a,d) with a-d = s */

        /* B = -ceil((s^2 + 1)/4) = -(s^2 + 4)/4  (integer division) */
        int64_t ssq = s * s;
        int64_t q = (ssq + 4) / 4;
        int64_t B = -q;                /* negative integer */

        int64_t countBC = 0;           /* number of (b,c) with -n<=b,c<=n and bc <= B */

        for (int64_t b = 1; b <= N; ++b) {
            int64_t val1 = floor_div(B, b);          /* floor(B / b)   */

            if (val1 >= -N)                           /* contribution from c negative */
                countBC += N + 1 + val1;              /* all negative c <= val1 */

            int64_t ceil_val = -val1;                 /* ceil(-B / b)   */
            if (ceil_val <= N)                         /* contribution from c positive */
                countBC += N - ceil_val + 1;
        }

        complexCount += weight * countBC;
    }

    return total - (seq_t)complexCount;
}
