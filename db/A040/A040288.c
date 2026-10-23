/*
 * A040288  Continued fraction for sqrt(306).
 * https://oeis.org/A040288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = ⌊√306⌋ = 17; for n ≥ 1, a(n) is the (n‑th) term of the
 *            continued fraction expansion of √306, which repeats the period
 *            {2,34}. The code computes the terms by the standard
 *            continued‑fraction recurrence
 * Range:     u8 none, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    const seq_t D = (seq_t)306;      /* discriminant */
    seq_t a0 = floor_sqrt(D);        /* floor(√D) */

    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a = a0;

    if (n == 0)
        return a;

    /* Compute a(1)...a(n) using the recurrence: */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t m2 = d * a - m;
        seq_t d2 = (D - m2 * m2) / d;
        seq_t a2 = (a0 + m2) / d2;

        m = m2;
        d = d2;
        a = a2;
    }
    return a;
}
