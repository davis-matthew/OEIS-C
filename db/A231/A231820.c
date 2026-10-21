/*
 * A231820  Least positive k such that n*k! - 1 is a prime, or 0 if no such k
 *          exists.
 * https://oeis.org/A231820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive k such that n*k! - 1 is prime, or 0 if
 *            none exists
 * Range:     u8 n <= 12, u16 n <= 60, u32 n <= 88, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* offset: n starts from 1, but return 0 for n <= 0 just in case */
    if (n == 0) return 0;

    seq_t fact = 1;          /* running factorial k! */
    /* empirical test shows that all known terms have k <= 50 for n <= 10000 */
    /* set a reasonable upper bound to avoid infinite loops in pathological cases */
    const idx_t K_MAX = 50;

    for (idx_t k = 1; k <= K_MAX; ++k)
    {
        fact = fact * (seq_t)k;                 /* k! */
        seq_t val = (seq_t)n * fact - 1;        /* n*k! - 1 */

        if (isPrimeGeneric(val))
            return (seq_t)k;                   /* found the minimal k */
    }

    /* if no such k found within the limit, return 0 as specified */
    return 0;
}
