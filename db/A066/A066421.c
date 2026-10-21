/*
 * A066421  a(n) = least k > 0 such that sigma^(k)(n) + 1 is prime, if such k
 *          exists; otherwise 0, where sigma^(k) denotes application of sigma
 *          k times.
 * https://oeis.org/A066421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 32, u32 n <= 9715, u64 n <= 9715,
 *            u128 n <= 9715, big any n
 * Helpers:   isPrimeGeneric, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = least k > 0 such that σ^k(n) + 1 is prime,
   otherwise 0.  Uses sigma_of and isPrimeGeneric from utils. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* safety for indices below offset */

    idx_t cur = n;
    seq_t k = 1;                 /* step counter (k > 0) */

    for (;;) {
        /* Apply σ once.  sigma_of returns a seq_t value. */
        seq_t m = sigma_of(cur);

        /* Candidate to test for primality. */
        seq_t cand = m + (seq_t)1;
        if (isPrimeGeneric(cand))
            return (seq_t)k;

        /* Stop if m cannot be represented as idx_t (overflow). */
        if (m > (seq_t)UINT64_MAX)
            break;

        /* Prepare for next iteration. */
        cur = (idx_t)m;
        k++;
    }

    /* No suitable k found within representable range. */
    return (seq_t)0;
}
