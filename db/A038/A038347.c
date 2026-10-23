/*
 * A038347  Sum of first n primes of form 4k-1.
 * https://oeis.org/A038347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of the first n primes congruent to 3 (mod 4)
 * Range:     u8 n <= 9, u16 n <= 109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"
#include "../utils/_prime_limit_for_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1, guard against n = 0 */

    /* Rough upper bound: about twice the nth prime; add a safety margin. */
    idx_t limit = _prime_limit_for_idx(n * 3) + 100;
    if (limit < 10)
        limit = 10;

    idx_t *primes = NULL;
    size_t prime_cnt = 0;

    while (1) {
        /* Generate all primes ≤ limit. */
        build_primes(limit, &primes, &prime_cnt);

        /* On allocation failure, return a safe value. */
        if (primes == NULL)
            return (seq_t)0;

        seq_t sum = 0;
        size_t count = 0;

        /* Sum the first n primes that are 3 mod 4. */
        for (size_t i = 0; i < prime_cnt && count < n; ++i) {
            if ((primes[i] & 3ULL) == 3ULL) {
                sum += (seq_t)primes[i];
                ++count;
            }
        }

        free(primes);          /* release memory before retrying */

        if (count >= n)
            return sum;         /* we have the required sum */

        /* Not enough qualifying primes – increase the limit and try again. */
        limit *= 2;
    }
}
