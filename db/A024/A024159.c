/*
 * A024159  Number of integer-sided triangles with sides a,b,c, a<b<c, a+b+c=n
 *          such that n divides a^3 + b^3 + c^3.
 * https://oeis.org/A024159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of integer‑sided triangles with a<b<c, a+b+c=n, and
 *            n | a^3 + b^3 + c^3 (exhaustive search over side lengths)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 3) return 0;                       /* no triangle possible */

    uint64_t count = 0;
    uint64_t n_mod = (uint64_t)n;

    idx_t half      = n / 2;
    idx_t max_a     = (n - 3) / 3;              /* a <= floor((n-3)/3) */

    for (idx_t a_val = 1; a_val <= max_a; ++a_val) {
        uint64_t a_mod = ((uint64_t)a_val * a_val * a_val) % n_mod;

        idx_t low_b   = half - a_val + 1;      /* ensures a+b > n/2 */
        if (low_b <= a_val) low_b = a_val + 1;/* ensures b > a      */
        idx_t high_b  = (n - a_val - 1) / 2;   /* ensures b < c      */

        if (low_b > high_b)
            continue;

        for (idx_t b_val = low_b; b_val <= high_b; ++b_val) {
            idx_t c_val = n - a_val - b_val;          /* guaranteed c_val > b_val */
            uint64_t b_mod = ((uint64_t)b_val * b_val * b_val) % n_mod;
            uint64_t c_mod = ((uint64_t)c_val * c_val * c_val) % n_mod;

            uint64_t sum_mod = a_mod + b_mod + c_mod;
            if (sum_mod % n_mod == 0)
                ++count;
        }
    }

    return (seq_t)count;
}
