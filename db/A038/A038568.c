/*
 * A038568  Numerators in canonical bijection from positive integers to
 *          positive rationals.
 * https://oeis.org/A038568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, core, easy, nice
 * Formula:   a(n) follows the canonical bijection enumeration described in
 *            OEIS A038568. For each integer m ≥ 2, let R(m) be the list of
 *            integers 1 ≤ r ≤ m that are coprime to m in increasing order.
 *            The sequence is produced by writing, for each m in increasing
 *            order, the pairs (r, m) for every r ∈ R(m). That...
 * Range:     u8 n <= 7886, u16 n <= 7886, u32 n <= 7886, u64 n <= 7886,
 *            u128 n <= 7886, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* index of the next term to output (0‑based) */
    idx_t idx = 1;          /* a(0) already produced */
    for (idx_t m = 2; ; ++m)   /* m runs over positive integers ≥2 */
    {
        for (idx_t k = 1; k <= m; ++k)  /* k runs over 1…m   */
        {
            if (gcd((seq_t)k, (seq_t)m) == (seq_t)1)  /* coprime check */
            {
                /* odd position – the coprime */
                if (idx == n)
                    return (seq_t)k;
                ++idx;

                /* even position – the integer m */
                if (idx == n)
                    return (seq_t)m;
                ++idx;
            }
        }
    }

    /* unreachable, but keep the compiler happy */
    return (seq_t)0;
}
