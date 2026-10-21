/*
 * A239628  Factored over the Gaussian integers, the least positive number
 *          having n prime factors counted multiply, including units -1, i,
 *          and -i.
 * https://oeis.org/A239628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer N>0 such that N has exactly n
 *            prime factors * in its Gaussian‐integer factorisation (counting
 *            units -1, i, -i, but not the unit 1 * except for N=1). N is
 *            represented as 2^a·3^b with 2a+b+δ = n, * where δ = 0 if a % 4
 *            == 0 and δ = 1 otherwise. The value is minimal * w...
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 1)                /* a(1) = 1, the only case where the factor 1 is counted */
        return (seq_t)1;

    seq_t best = 0;            /* 0 indicates that a candidate has not been stored yet */

    /* a runs from 0 up to floor(n/2) because 2a+δ ≤ n. */
    idx_t max_a = n / 2;
    for (idx_t a = 0; a <= max_a; ++a) {
        idx_t delta = (a % 4 == 0) ? 0 : 1;
        if (n < 2 * a + delta) continue;          /* no non‑negative b is possible */

        idx_t b = n - 2 * a - delta;              /* b ≥ 0 by construction */

        /* Compute 2^a and 3^b using the supplied big‑int routine. */
        seq_t p2 = pow_uint(2, a);
        seq_t p3 = pow_uint(3, b);

        seq_t candidate = p2 * p3;                /* product is integer value of N */

        if (best == 0 || candidate < best)
            best = candidate;
    }

    return best;               /* guaranteed to be a valid value for n ≥ 2 */
}
