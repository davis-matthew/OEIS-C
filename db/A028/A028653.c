/*
 * A028653  Expansion of (theta_3(z)*theta_3(22z)+theta_2(z)*theta_2(22z)).
 * https://oeis.org/A028653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer pairs (a,b) with a^2 + 22·b^2 = n and a
 *            ≡ b (mod 2)
 * Range:     u8 n <= 63, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    uint64_t limit = n / 22;                 /* maximum possible value of b^2 */
    idx_t bmax = 0;
    while ((bmax + 1) * (bmax + 1) <= limit) ++bmax;   /* floor(sqrt(limit)) */

    seq_t n_seq = (seq_t)n;
    for (idx_t b = 0; b <= bmax; ++b) {
        seq_t u = n_seq - (seq_t)22 * (seq_t)(b * b);
        seq_t s = floor_sqrt(u);
        if (s * s != u) continue;           /* u is not a perfect square */
        if ((s & 1) != (b & 1)) continue;     /* a and b must have same parity */

        if (b == 0) {                        /* b = 0 */
            count += (s == 0) ? 1 : 2;
        } else {                              /* b > 0 */
            count += (s == 0) ? 2 : 4;        /* a=±s, b=±b when s>0; a=0, b=±b when s=0 */
        }
    }
    return count;
}
