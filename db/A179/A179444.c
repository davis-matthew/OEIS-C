/*
 * A179444  Composite numbers m of the form m = 10k + 1 where k is also
 *          composite.
 * https://oeis.org/A179444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth composite number of the form 10k+1 where k is
 *            composite. Computation: brute-force search
 * Range:     u8 n <= 7, u16 n <= 4236, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: Uses the small-prime test, which is adequate for the size of numbers
   involved in the first many terms (k and m are below 1M for n=10 000). */
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 2; ; ++k) {
        seq_t m = (seq_t)10 * k + 1;
        /* k>1 is automatic since k starts at 2 */
        if (!is_prime_small((seq_t)k) &&            /* k is composite */
            !is_prime_small(m)) {                   /* m is composite */
            ++count;
            if (count == n)
                return m;
        }
    }
}
