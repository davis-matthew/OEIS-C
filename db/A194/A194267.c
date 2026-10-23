/*
 * A194267  Smallest sum of three distinct primes of the form n*k+1.
 * https://oeis.org/A194267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sum of the smallest three distinct primes congruent to 1 (mod n)
 * Range:     u8 n <= 12, u16 n <= 520, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"

/* Compute a(n) for n >= 1 (a(0) is undefined, returns 0).  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t sum   = 0;
    int   found = 0;
    for (idx_t k = 0; ; ++k)
    {
        idx_t cand = n * k + 1;      /* candidate prime of the form n*k+1 */
        if (cand < 2) continue;     /* skip numbers less than 2 */

        if (is_prime(cand))
        {
            sum += (seq_t)cand;      /* truncation is fine if sum exceeds seq_t */
            if (++found == 3) break; /* found the three smallest distinct primes */
        }
    }

    return sum;
}
