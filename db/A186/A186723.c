/*
 * A186723  a(n) = n^n! (mod 10).
 * https://oeis.org/A186723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^(n!) mod 10
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base = n mod 10 */
    unsigned int base = (unsigned int)(n % 10U);

    /* trivial cases where the result does not depend on the exponent */
    if (base == 0) return (seq_t)0;
    if (base == 1) return (seq_t)1;
    if (base == 5) return (seq_t)5;
    if (base == 6) return (seq_t)6;

    /* cycle length of the exponent in modulo 10 */
    unsigned int cycle;
    if (base == 4)                 /* 4^k mod 10 cycles with period 2 */
        cycle = 2;
    else                            /* all other bases coprime with 10 have period 4 */
        cycle = 4;

    /* exponent e = n! modulo the cycle length */
    unsigned int exp_mod;
    if (n < cycle) {
        /* compute n! mod cycle (cycle is at most 4, so this is trivial) */
        unsigned int fact = 1;
        for (unsigned int i = 1; i <= (unsigned int)n; ++i)
            fact = (fact * i) % cycle;
        exp_mod = fact;
    } else {
        /* for n >= cycle, n! is divisible by cycle */
        exp_mod = 0;
    }

    /* if exponent is a multiple of the cycle, use the cycle itself */
    if (exp_mod == 0)
        exp_mod = cycle;

    /* compute base^exp_mod mod 10 (exp_mod is 2 or 4, at most 4) */
    unsigned int result = 1;
    for (unsigned int i = 0; i < exp_mod; ++i)
        result = (result * base) % 10U;

    return (seq_t)result;
}
