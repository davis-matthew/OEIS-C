/*
 * A055526  Shortest hypotenuse of a Pythagorean triangle with n as length of
 *          a leg.
 * https://oeis.org/A055526
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt(n^2 + A055527(n)^2) Implementation: find the
 *            smallest positive integer a such that a*(a+2n) is a perfect
 *            square; the minimal hypotenuse is then n + a. This is equivalent
 *            to the standard characterization of Pythagorean triples with leg
 *            n
 * Range:     u8 n <= 22, u16 n <= 366, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is not defined; return 0 as a default. */
    if (n < 3) return (seq_t)0;

    seq_t best_a = (seq_t)-1;                     /* sentinel = maximum value */
    seq_t two_n = (seq_t)2 * (seq_t)n;            /* 2n in seq_t */

    /* Enumerate all divisors g of 2n. */
    for (idx_t g = 1; (seq_t)g * (seq_t)g <= two_n; ++g) {
        if (two_n % (seq_t)g != 0) continue;
        /* Process g and its paired divisor two_n/g */
        idx_t g_list[2] = { g, (idx_t)(two_n / (seq_t)g) };
        for (int j = 0; j < 2; ++j) {
            seq_t gval = (seq_t)g_list[j];
            seq_t D = two_n / gval;              /* D = 2n / g */

            /* For each divisor d of D, look for factor pairs (d1,d2). */
            for (idx_t d = 1; (seq_t)d * (seq_t)d <= D; ++d) {
                if (D % (seq_t)d != 0) continue;
                idx_t d1 = d;
                idx_t d2 = (idx_t)(D / (seq_t)d);
                /* d1 < d2 is automatically satisfied for d < sqrt(D);
                   when d1 == d2 we skip because u would be 0. */
                if (d1 == d2) continue;
                /* Parity check: d1 and d2 must have the same parity. */
                if (((d1 ^ d2) & 1) != 0) continue;

                /* Compute u = (d2 - d1)/2, ensuring u > 0. */
                seq_t u = ((seq_t)d2 - (seq_t)d1) / (seq_t)2;
                if (u == 0) continue;

                /* a_candidate = g * u^2. */
                seq_t a_candidate = gval * u * u;
                if (best_a == (seq_t)-1 || a_candidate < best_a) {
                    best_a = a_candidate;
                }
            }
        }
    }

    /* best_a should have been found; return the minimal hypotenuse. */
    return (seq_t)n + best_a;
}
