/*
 * A217462  a(n) is the sum of total number of nonnegative integer solutions
 *          to each of a^2 + b^2 = n, a^2 + 2*b^2 = n, a^2 + 3*b^2 = n and a^2
 *          + 7*b^2 = n. (Order does not matter for the equation a^2+b^2 = n).
 * https://oeis.org/A217462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k∈{1,2,3,7}} r_k(n), where r_k(n) is the number of
 *            non‑negative integer solutions to a² + k·b² = n, and for k = 1
 *            solutions are counted unordered
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at n = 1 */

    seq_t sum = 0;

    /* maximum value of a with a² ≤ n */
    idx_t max_a = 0;
    while ((max_a + 1) * (max_a + 1) <= n) {
        ++max_a;
    }

    for (idx_t a_val = 0; a_val <= max_a; ++a_val) {
        idx_t a2  = a_val * a_val;
        idx_t rem = n - a2;           /* remaining part for k·b² */

        /* --- k = 1, unordered pairs (a,b) --- */
        idx_t s = 0;
        while (s * s <= rem) ++s;
        --s;                         /* floor(sqrt(rem)) */
        if (s * s == rem && a_val <= s) {
            ++sum;
        }

        /* --- k = 2, ordered pairs --- */
        if ((rem & 1) == 0) {        /* rem even */
            idx_t t = rem >> 1;      /* rem / 2 */
            s = 0;
            while (s * s <= t) ++s;
            --s;
            if (s * s == t) ++sum;
        }

        /* --- k = 3, ordered pairs --- */
        if (rem % 3 == 0) {
            idx_t t = rem / 3;
            s = 0;
            while (s * s <= t) ++s;
            --s;
            if (s * s == t) ++sum;
        }

        /* --- k = 7, ordered pairs --- */
        if (rem % 7 == 0) {
            idx_t t = rem / 7;
            s = 0;
            while (s * s <= t) ++s;
            --s;
            if (s * s == t) ++sum;
        }
    }

    return sum;
}
