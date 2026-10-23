/*
 * A136799  Last term in a sequence of at least 3 consecutive composite
 *          integers.
 * https://oeis.org/A136799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 5680, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   create_prime_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A136799: Last term in a run of at least three consecutive composite integers
 * (OEIS offset 1, a(1) = 10).  The implementation searches for consecutive
 * composite numbers, counting runs of length ≥ 3, and returns the last element
 * of the requested run.  It uses a simple sieving routine that grows the
 * search range until the nth run is found.  Works for all supported idx_t
 * and seq_t types. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/create_prime_mask.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, safe fallback */

    /* Rough upper bound for the sieve.  The sequence grows ~ n log n,
     * so a factor of 20 is comfortably larger than needed for n ≤ 10 000.
     * We will double the limit if the nth run is not yet found. */
    idx_t limit = n * 20 + 100;
    while (true) {
        bool *is_prime = create_prime_mask(limit);
        if (!is_prime) return 0;              /* allocation failure */

        idx_t count = 0;            /* number of completed runs   */
        idx_t consec = 0;           /* current run length         */
        idx_t last_comp = 0;        /* last composite seen         */

        for (idx_t m = 2; m <= limit; ++m) {
            if (!is_prime[m]) {                /* composite */
                ++consec;
                last_comp = m;
            } else { /* prime */
                if (consec >= 3) {
                    ++count;
                    if (count == n) {
                        free(is_prime);
                        return (seq_t)last_comp;
                    }
                }
                consec = 0;                    /* reset run length */
            }
        }

        /* If the loop ends without finding the nth run, extend the sieve */
        free(is_prime);
        limit = limit * 2 + 100;              /* safe growth factor   */
    }
}
