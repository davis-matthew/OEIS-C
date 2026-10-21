/*
 * A081885  Number of steps needed to reach an integer starting with n+1/16
 *          and iterating the map x-->x*ceiling(x).
 * https://oeis.org/A081885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps needed to reach an integer starting with
 *            n + 1/16 and iterating the map x -> x * ceil(x)
 * Range:     u8 none, u16 n <= 3, u32 n <= 69, u64 n <= 101, u128 n <= 101,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* All calculations are performed in the type seq_t,
       which may be an unsigned integer type or an arbitrary‑precision
       integer class.  The function uses the provided gcd() helper
       and reduces the fraction at each step. */
    seq_t num = (seq_t)n * 16 + 1;   /* numerator 16*n + 1 */
    seq_t den = 16;                  /* denominator 16 */
    seq_t steps = 0;

    /* Loop until the rational is an integer (denominator becomes 1). */
    while (den != 1) {
        /* c = ceil(num / den) = (num + den - 1) / den   (exact integer). */
        seq_t c = (num + den - 1) / den;
        num = num * c;              /* numerator of the next iterate */
        /* Denominator stays the same after multiplication by an integer. */
        /* Reduce by gcd to keep numbers as small as possible. */
        seq_t g = gcd(num, den);
        num = num / g;
        den = den / g;
        steps = steps + 1;
    }
    return steps;
}
