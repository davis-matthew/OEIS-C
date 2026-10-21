/*
 * A226973  Difference between n! and the largest cube < n!.
 * https://oeis.org/A226973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - floor((n!)^(1/3))^3 * (OEIS A226973)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   floor_cube_root, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_cube_root.h"
#include "../utils/seq_pow.h"

/* Compute factorial using 128‑bit arithmetic when possible,
   falling back to seq_t multiplication for larger n.  The
   128‑bit method works for all builds and avoids overflow in
   the 64‑bit case for n ≤ 34 (where the final result still
   fits in uint64_t).  For n > 34 the factorial is computed
   with seq_t; this path is only exercised when seq_t can
   represent the exact factorial (i.e., the arbitrary‑precision
   build). */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* OEIS offset 1 */

    /* ----- small n: use 128‑bit intermediate ----- */
    if (n <= 34) {
        unsigned __int128 fact = 1;
        for (idx_t i = 2; i <= n; ++i)
            fact *= (unsigned __int128)i;

        /* binary search for floor(cuberoot(fact)) */
        unsigned __int128 lo = 0, hi = fact;
        while (lo < hi) {
            unsigned __int128 mid = (lo + hi + 1) >> 1;
            unsigned __int128 cube = mid * mid * mid;
            if (cube <= fact) lo = mid;
            else               hi = mid - 1;
        }
        unsigned __int128 m   = lo;
        unsigned __int128 cube = m * m * m;
        unsigned __int128 diff = fact - cube;
        return (seq_t)diff;
    }

    /* ----- larger n: rely on seq_t for exact arithmetic ----- */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    seq_t m   = floor_cube_root(fact);
    seq_t cube = seq_pow(m, 3);
    return fact - cube;
}
