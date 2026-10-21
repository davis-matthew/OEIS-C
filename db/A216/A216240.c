/*
 * A216240  Composite numbers arising in Eratosthenes sieve with removing the
 *          multiples of every other remaining numbers after 2 (see comment).
 * https://oeis.org/A216240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 3806, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calloc_uc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A216240:  sequence of odd composite numbers that survive the
 * modified Eratosthenes sieve.
 * 
 * Process:
 * 1. Remove all even numbers greater than 2 (we keep 2).
 * 2. From the remaining odd numbers in increasing order,
 *    treat every second surviving number as a "remover".
 *    When a number r becomes a remover, delete all remaining odd
 *    multiples of r (except r itself).  The remover r itself stays.
 * 3. After all removals, the surviving odd composite numbers
 *    are listed in increasing order.
 *
 * The function below computes the nth term (1‑based) by simulating
 * the above sieve.  It grows the search bound until at least n
 * composite survivors are found.  No cached data is preserved
 * between calls, but the algorithm is efficient enough for n≤10000.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/calloc_uc.h"

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for offset 0 */
        return (seq_t)0;

    idx_t target = n;
    /* initial bound – adjust upward until enough survivors are found */
    idx_t limit = target < 50 ? 50 : target * 20;

    while (1) {
        /* ---------------------------------------------------------- */
        /* 1. Allocate arrays                                       */
        /* ---------------------------------------------------------- */
        size_t sz = (size_t)limit + 1;          /* index 0..limit */
        unsigned char *alive   = calloc_uc(sz);  /* odd numbers survivor flag   */
        unsigned char *is_prime = calloc_uc(sz); /* prime flag: 1 = prime, 0 = composite   */
        if (!alive || !is_prime) {
            free(alive); free(is_prime);
            return (seq_t)0;
        }

        /* ---------------------------------------------------------- */
        /* 2. Initialise 'alive' with all odd numbers ≥3            */
        /* ---------------------------------------------------------- */
        for (idx_t i = 3; i <= limit; i += 2)
            alive[i] = 1;

        /* ---------------------------------------------------------- */
        /* 3. Eliminate multiples of every second surviving number   */
        /* ---------------------------------------------------------- */
        bool need_removal = false;
        for (idx_t i = 3; i <= limit; i += 2) {
            if (!alive[i]) continue;        /* already removed */
            if (!need_removal) {            /* first of the pair */
                need_removal = true;
            } else {                        /* second of the pair – the remover */
                idx_t r = i;
                /* remove odd multiples of r, starting from 3r */
                for (idx_t m = r * 3; m <= limit; m += r * 2)
                    alive[m] = 0;
                need_removal = false;
            }
        }

        /* ---------------------------------------------------------- */
        /* 4. Simple prime sieve to identify composites               */
        /* ---------------------------------------------------------- */
        for (idx_t i = 2; i <= limit; ++i)
            is_prime[i] = 1;
        for (idx_t p = 2; p * p <= limit; ++p) {
            if (is_prime[p]) {
                for (idx_t j = p * p; j <= limit; j += p)
                    is_prime[j] = 0;
            }
        }

        /* ---------------------------------------------------------- */
        /* 5. Scan survivors and count composites                     */
        /* ---------------------------------------------------------- */
        idx_t count = 0;
        for (idx_t i = 3; i <= limit; i += 2) {
            if (alive[i] && !is_prime[i]) {   /* composite survivor */
                ++count;
                if (count == target) {
                    seq_t result = (seq_t)i;
                    free(alive);
                    free(is_prime);
                    return result;
                }
            }
        }

        /* ---------------------------------------------------------- */
        /* 6. Not enough terms – enlarge limit and retry             */
        /* ---------------------------------------------------------- */
        free(alive);
        free(is_prime);
        limit *= 2;
        if (limit == 0)  /* overflow protection */
            return (seq_t)0;
    }
}
