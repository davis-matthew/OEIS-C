/*
 * A121898  Triangular numbers that are sandwiched between two semiprimes; or
 *          triangular numbers t such that t-1 and t+1 are both semiprime.
 * https://oeis.org/A121898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th triangular number t such that t‑1 and t+1 * are
 *            semiprime (each has exactly two prime factors, counted with
 *            multiplicity)
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* out of range; should not occur */

    idx_t k      = 1;              /* current index for the triangular numbers */
    idx_t found  = 0;              /* number of qualifying triangular numbers found */

    while (found < n) {
        seq_t k_seq = (seq_t)k;             /* promote to seq_t for multiplication */
        seq_t t     = k_seq * (k_seq + 1) / 2;

        /* test whether t‑1 and t+1 are semiprime */
        if (count_prime_factors(t - 1) == 2 &&
            count_prime_factors(t + 1) == 2) {
            ++found;
            if (found == n)
                return t;               /* found the requested term */
        }
        ++k;
    }
    return 0; /* unreachable but keeps the function defined for all control paths */
}
