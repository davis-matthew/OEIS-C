/*
 * A155929  Integers a(n) such that cube(n)-square(n)-1 and
 *          cube(n)+square(n)+1 are primes.
 * https://oeis.org/A155929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer m such that m³−m²−1 and m³+m²+1 are
 *            prime
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 34, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n ≤ 0 */

    idx_t  found = 0;                         /* how many good numbers seen */
    seq_t  m = 2;                             /* candidate value */

    while (1)
    {
        seq_t sq   = m * m;                   /* m² */
        seq_t cu   = sq * m;                  /* m³ */
        seq_t left = cu - sq - (seq_t)1;      /* m³ − m² − 1 */
        seq_t right= cu + sq + (seq_t)1;      /* m³ + m² + 1 */

        if (isPrimeGeneric(left) && isPrimeGeneric(right))
        {
            ++found;
            if (found == n)
                return m;
        }
        ++m;                                  /* test next integer */
    }
}
