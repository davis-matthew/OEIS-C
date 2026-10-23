/*
 * A178697  Expansion of the polynomial
 *          (1+x^3)*(1+x^5)*(1+x^7)*(1+x^9)*(1+x^11)*(1+x^13)*(1+x^15)*(1+x^17)*(1+x^19)*(1+x^21).
 * https://oeis.org/A178697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in
 *            (1+x^3)(1+x^5)(1+x^7)(1+x^9)(1+x^11) *
 *            (1+x^13)(1+x^15)(1+x^17)(1+x^19)(1+x^21)
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Highest possible exponent is 3+5+…+21 = 120.  */
    static seq_t cache[121];
    static bool initialized = false;

    if (!initialized) {
        /* initialise all entries to zero */
        for (size_t i = 0; i < 121; ++i)
            cache[i] = (seq_t)0;
        cache[0] = (seq_t)1;

        const idx_t exps[10] = {3,5,7,9,11,13,15,17,19,21};

        /* dynamic programming – each exponent may be used at most once */
        for (int ei = 0; ei < 10; ++ei) {
            idx_t e = exps[ei];
            for (size_t s = 120; s >= (size_t)e; --s)
                cache[s] += cache[s - (size_t)e];
        }
        initialized = true;
    }

    if (n > 120)
        return (seq_t)0;
    return cache[(size_t)n];
}
