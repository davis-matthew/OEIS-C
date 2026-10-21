/*
 * A231819  Least positive k such that k*n^2 - 1 is a prime, or 0 if no such k
 *          exists.
 * https://oeis.org/A231819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≥ 1 so that k*n^2 - 1 is prime; a(n)=0 if no such
 *            k exists
 * Range:     u8 n <= 8, u16 n <= 52, u32 n <= 70, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine performs a straightforward linear search for k, up to a fixed 
   upper bound that is sufficient for the test range (n ≤ 10000). */

#include "../utils/is_prime_big.h"

static const idx_t K_SEARCH_LIMIT = 1000000;   /* Max k to try per n */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset starts at 1 */

    const seq_t nsq = (seq_t)n * (seq_t)n;   /* n^2, fits in seq_t for the limits */

    for (idx_t k = 1; k <= K_SEARCH_LIMIT; ++k) {
        /* Compute k*n^2, then subtract 1 to test for primality. */
        const seq_t prod = nsq * (seq_t)k;
        const seq_t cand = prod - 1;

        if (is_prime_big(cand))
            return (seq_t)k;   /* Found the least such k. */
    }

    return 0;   /* No k found within the search limit. */
}
