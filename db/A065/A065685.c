/*
 * A065685  Number of primes <= prime(n) which begin with a 6.
 * https://oeis.org/A065685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, prime_limit_for_idx, starts_with_six
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of primes ≤ prime(n) whose decimal representation starts with 6 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* forward declaration of the helper that supplies primes */
#include "../utils/build_primes_upto.h"
#include "../utils/prime_limit_for_idx.h"
#include "../utils/starts_with_six.h"

/* the sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* outside the offset – harmless */

    /* static cache of primes and of prefix counts */
    static uint64_t *primes   = NULL;
    static size_t    primes_cnt = 0;
    static seq_t   *prefix   = NULL;   /* prefix[i] = count of primes ≤ prime(i)
                                        * starting with 6, allocation size = primes_cnt+1
                                        */

    /* enlarge the cache until it contains at least n primes */
    if (primes == NULL || n > primes_cnt) {
        idx_t limit = prime_limit_for_idx(n);

        for (;;) {
            /* free old data if any */
            if (primes != NULL)   free(primes);
            if (prefix != NULL)   free(prefix);

            /* obtain all primes ≤ limit */
            build_primes_upto(limit, &primes, &primes_cnt);

            if (primes_cnt >= n)
                break;                           /* we have enough */

            /* not enough – double the limit and try again */
            limit *= 2;
        }

        /* build the prefix array */
        prefix = (seq_t *)calloc(primes_cnt + 1, sizeof(seq_t));
        if (prefix == NULL)
            exit(EXIT_FAILURE);

        prefix[0] = (seq_t)0;
        for (size_t i = 1; i <= primes_cnt; ++i) {
            seq_t add = starts_with_six(primes[i - 1]) ? (seq_t)1 : (seq_t)0;
            prefix[i] = prefix[i - 1] + add;
        }
    }

    return prefix[(size_t)n];
}
