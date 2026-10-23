/*
 * A185004  Ramanujan modulo primes R_(3,1)(n): a(n) is the smallest number
 *          such that if x >= a(n), then pi_(3,1)(x) - pi_(3,1)(x/2) >= n,
 *          where pi_(3,1)(x) is the number of primes==1 (mod 3) <= x.
 * https://oeis.org/A185004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the smallest x such that
 *           pi_{3,1}(x) - pi_{3,1}(x/2) >= n
 *   where pi_{3,1}(x) counts primes congruent to 1 mod 3 up to x.
 *   The function computes this definition by scanning from a
 *   sufficiently large bound downward, storing the running minimum
 *   of the difference, and then finding the smallest index whose
 *   future minimum is at least n.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <limits.h>

static size_t estimate_limit(idx_t n)
{
    if (n < 6) return 300;  /* safe lower bound for tiny n */
    double d = 4.0 * (double)n;
    double bound = d * (log(d) + log(log(d)));
    /* add safety factor */
    bound *= 1.2;
    bound += 1000.0;
    return (size_t)bound;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but return 0 safely */

    size_t limit = estimate_limit(n);
    while (1) {
        /* Allocate and build sieve of primes up to 'limit' */
        uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
        if (!is_prime) return 0;  /* allocation failure */
        memset(is_prime, 1, limit + 1);
        if (limit >= 0) is_prime[0] = 0;
        if (limit >= 1) is_prime[1] = 0;

        for (size_t i = 2; i * i <= (size_t)limit; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= (size_t)limit; j += i)
                    is_prime[j] = 0;
            }
        }

        /* Compute prefix counts of primes == 1 mod 3 */
        uint32_t *pi1 = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
        if (!pi1) {
            free(is_prime);
            return 0;
        }
        pi1[0] = 0;
        for (size_t i = 1; i <= limit; ++i) {
            pi1[i] = pi1[i - 1] + ((is_prime[i] && (i % 3 == 1)) ? 1 : 0);
        }

        /* Allocate array for running minimum of the difference */
        uint32_t *min_future = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
        if (!min_future) {
            free(pi1);
            free(is_prime);
            return 0;
        }

        uint32_t cur_min = UINT32_MAX;
        for (size_t x = limit; x > 0; --x) {
            uint32_t val = pi1[x] - pi1[x / 2];
            if (val < cur_min) cur_min = val;
            min_future[x] = cur_min;
        }

        free(pi1);
        free(is_prime);

        /* Search for the smallest x satisfying the condition */
        seq_t answer = 0;
        for (size_t x = 1; x <= limit; ++x) {
            if (min_future[x] >= (uint32_t)n) {
                answer = (seq_t)x;
                break;
            }
        }
        free(min_future);

        if (answer != 0) return answer;  /* found within current limit */

        /* Limit too small; expand and try again */
        limit = limit * 2 + 1000;
    }
}
