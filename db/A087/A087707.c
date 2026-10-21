/*
 * A087707  Number of steps for iteration of map x -> (5/3)*ceiling(x) to
 *          reach an integer > n when started at n, or -1 if no such integer
 *          is ever reached.
 * https://oeis.org/A087707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of x → (5/3)·ceil(x) required to
 *            reach an integer > n when starting from n; returns –1 if never
 *            reached
 * Range:     u8 n <= 10, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;          /* offset guard */

    seq_t num = n;        /* current numerator of the rational number */
    seq_t den = 1;        /* denominator (1 for the initial integer) */
    idx_t steps = 0;
    const idx_t MAX_STEPS = 20000;           /* safety limit */

    while (steps < MAX_STEPS) {
        /* ceil(num/den) */
        seq_t ceil = (num + den - 1) / den;

        /* next rational = 5 * ceil / 3 */
        num = ceil * (seq_t)5;
        den = (seq_t)3;          /* denominator stays 3 after the first step */

        ++steps;

        /* check if we have an integer and if it exceeds the start value */
        if (num % den == 0) {
            seq_t val = num / den;
            if (val > (seq_t)n) {
                return (seq_t)steps;
            }
        }
    }
    return (seq_t)-1;           /* should never happen under the conjecture */
}
