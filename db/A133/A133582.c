/*
 * A133582  a(n) is found from a(n-1) by dividing by D-1 and multiplying by D,
 *          where D is the smallest number that is not a divisor of a(n-1).
 * https://oeis.org/A133582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1 let D be the smallest integer ≥2 that does
 *            NOT divide a(n‑1). Then a(n) = (a(n‑1)/(D-1)) * D. This
 *            implements the recurrence described in the OEIS entry for
 *            A133582
 * Range:     u8 n <= 13, u16 n <= 31, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined starting at offset 1; out‑of‑range
           values are undefined.  Return 0 for safety. */
        return 0;
    }

    seq_t value = 1;          /* a(1) = 1 */
    if (n == 1)
        return value;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t d = 2;          /* start looking for the smallest non‑divisor */
        while (value % d == 0)
            ++d;              /* keep incrementing until d does not divide */
        /* (d‑1) divides value by construction, so the division is exact */
        value = (value / (d - 1)) * d;
    }

    return value;
}
