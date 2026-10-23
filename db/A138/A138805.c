/*
 * A138805  Theta series of quadratic form x^2 + x*y + 7*y^2.
 * https://oeis.org/A138805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = # {(i,j) ∈ ℤ² : i*i + i*j + 7*j*j = n} (theta series of
 *            x² + x*y + 7*y²)
 * Range:     u8 n <= 15, u16 n <= 3868, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* External linkage as requested */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;  /* (0,0) only */

    /* count of representations */
    seq_t count = (seq_t)0;

    /* bound for |j|: j² ≤ 4n/27  →  |j| ≤ floor_sqrt(4n/27)                */
    idx_t jmax = (idx_t)floor_sqrt((seq_t)((4ULL * n) / 27ULL));

    /* loop over all integer j in the allowed range */
    for (long long j = -(long long)jmax; j <= (long long)jmax; ++j)
    {
        /* use absolute value to avoid signed overflow in j*j   */
        idx_t j_abs = (idx_t)(j >= 0 ? j : -j);
        seq_t j_sq = (seq_t)j_abs * (seq_t)j_abs;          /* j² */

        seq_t four_n   = (seq_t)4 * (seq_t)n;              /* 4n   */
        seq_t zs      = (seq_t)27 * j_sq;                 /* 27*j²   */

        if (four_n < zs)               /* discriminant negative -> skip */
            continue;

        seq_t delta = four_n - zs;     /* Δ = 4n – 27j² ≥ 0 */

        seq_t sqrt_d = floor_sqrt(delta);          /* floor(sqrt(Δ))   */
        if (sqrt_d * sqrt_d != delta)               /* not a perfect square */
            continue;

        /* parity condition: (-j ± sqrt_d) must be even → sqrt_d ≡ j (mod 2) */
        if (((unsigned long long)(sqrt_d & (seq_t)1)) != ((unsigned long long)(j & 1)))
            continue;

        count += (sqrt_d == (seq_t)0) ? (seq_t)1 : (seq_t)2;  /* one or two solutions */
    }

    return count;
}
