/*
 * A231917  Numbers n such that 16*n^2 + 10*n + 1 divides 2^n - 1.
 * https://oeis.org/A231917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m ≥ 0 such that 16*m^2 + 10*m + 1 divides
 *            2^m – 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Counter for found numbers. */
    idx_t count = 0;
    /* Iterate m from 0 upward until the nth solution is found. */
    for (idx_t m = 0; ; ++m) {
        /* Compute D = 16*m^2 + 10*m + 1.
           Use a larger intermediate type to avoid overflow of a* a where
           a is seq_t (possible for 64‑bit builds). 32‑bit and 16‑bit
           arithmetic is safe for the required ranges. */
        seq_t mt = (seq_t)m;
        seq_t D = 16 * mt * mt + 10 * mt + 1;

        /* BuildCheck: 2^m – 1 ≡ 0 (mod D) ⇔ 2^m ≡ 1 (mod D),
           except when D == 1 (then the congruence is always true). */
        bool ok;
        if (D == 1) {
            ok = true;                     /* 0 mod 1 == 0 */
        } else {
            seq_t r = mod_pow((seq_t)2, (uint64_t)m, D);
            ok = (r == 1);
        }

        if (ok) {
            ++count;
            if (count == n)
                return mt;
        }
    }
}
