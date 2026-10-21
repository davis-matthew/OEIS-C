/*
 * A224921  Number of Pythagorean triples (a, b, c) with a^2 + b^2 = c^2 and 0
 *          < a < b < c < n.
 * https://oeis.org/A224921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Pythagorean triples (a,b,c) with * 0 < a < b <
 *            c < n * The function computes the count by generating all
 *            primitive * triples using Euclid's formula and adding all of
 *            their non‑primitive * multiples whose hypotenuse is less than n
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* For n <= 3 there is no triple */
    if (n <= 3)
        return 0;

    seq_t total = 0;
    idx_t limit = n - 1;          /* we want c < n */
    /* m must satisfy m*m + k*k < n, so we can restrict m < sqrt(n) */
    for (idx_t m = 2; (m * m) < n; ++m)
    {
        for (idx_t k = 1; k < m; ++k)
        {
            /* Primitive condition: opposite parity and coprime */
            if (((m & 1u) != (k & 1u)) && (gcd((seq_t)m, (seq_t)k) == 1))
            {
                seq_t c0 = (seq_t)m * m + (seq_t)k * k; /* primitive hypotenuse */
                if (c0 >= n)            /* all multiples will be >= n */
                    continue;

                /* Number of multiples of c0 that are < n */
                seq_t mult_limit = (seq_t)limit / c0;
                total += mult_limit;
            }
        }
    }
    return total;
}
