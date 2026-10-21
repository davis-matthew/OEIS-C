/*
 * A097203  Number of 4-tuples (a,b,c,d) with 1 <= a <= b <= c <= d,
 *          a^2+b^2+c^2+d^2 = n and gcd(a,b,c,d) = 1.
 * https://oeis.org/A097203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4‑tuples (a,b,c,d) with 1 ≤ a ≤ b ≤ c ≤ d, a² +
 *            b² + c² + d² = n and gcd(a,b,c,d) = 1
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/floor_sqrt.h"

/* Count primitive representations of n as sum of four non‑zero squares
   with non‑decreasing order.  n is 1‑based (a(1) is first term). */
seq_t a(idx_t n) {
    if (n < 1) return 0;                     /* offset check */
    if ((n & 7) == 0) return 0;              /* 8 | n → no primitive solution */

    seq_t count = 0;
    /* precompute sqrt thresholds using integer sqrt via floor_sqrt */
    idx_t max_a = (idx_t)floor_sqrt((seq_t)n);
    for (idx_t a = 1; a <= max_a; ++a) {
        idx_t a2 = a * a;
        if (a2 > n) break;
        idx_t rem_a = n - a2;
        idx_t max_b = (idx_t)floor_sqrt((seq_t)rem_a);
        for (idx_t b = a; b <= max_b; ++b) {
            idx_t b2 = b * b;
            idx_t rem_ab = rem_a - b2;
            if (rem_ab < 0) break;
            idx_t max_c = (idx_t)floor_sqrt((seq_t)rem_ab);
            for (idx_t c = b; c <= max_c; ++c) {
                idx_t c2 = c * c;
                idx_t rem_abc = rem_ab - c2;
                if (rem_abc < 0) break;
                /* d must satisfy d² = rem_abc and d ≥ c */
                seq_t d_sq = floor_sqrt((seq_t)rem_abc);
                if (d_sq * d_sq != rem_abc) continue;   /* not a perfect square */
                idx_t d = (idx_t)d_sq;
                if (d < c) continue;                     /* enforce ordering */
                /* primitive test: gcd(a,b,c,d) == 1 */
                seq_t g = gcd((seq_t)a, (seq_t)b);
                g = gcd(g, (seq_t)c);
                g = gcd(g, (seq_t)d);
                if (g == 1) ++count;
            }
        }
    }
    return count;
}
