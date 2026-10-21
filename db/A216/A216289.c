/*
 * A216289  Smallest k in which there are exactly n primes between 10*k and
 *          10*k+9.
 * https://oeis.org/A216289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = smallest k such that the number of primes in the interval
 *            [10k, 10k+9] equals n. If no such k exists (n > 4), the function
 *            returns 0. This is computed by scanning decades incrementally
 *            and recording the first occurrence of each prime count that
 *            occurs
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t first_k[5] = {0};
static bool has_k[5] = {false};
static idx_t next_k = 0;

seq_t a(idx_t n)
{
    /* For n > 4 no decade can contain more than 4 primes.
       The convention for such cases is to return 0. */
    if (n > 4) {
        return (seq_t)0;
    }

    /* If we have already found the value for this n, return it. */
    if (has_k[n]) {
        return first_k[n];
    }

    /* Scan successive decades until we find the smallest k with n primes. */
    while (true) {
        idx_t start = (idx_t)10 * next_k;
        int primes = 0;

        /* Count primes in the decade [10k, 10k+9] */
        for (int offset = 0; offset <= 9; ++offset) {
            if (is_prime(start + offset)) {
                ++primes;
            }
        }

        /* Record the first k for any prime count seen that hasn't been
           seen before. */
        if (primes <= 4 && !has_k[primes]) {
            has_k[primes] = true;
            first_k[primes] = (seq_t)next_k;
        }

        /* If this is the count requested, return the k. */
        if (primes == n) {
            seq_t result = first_k[n];
            ++next_k;  /* advance for future calls */
            return result;
        }

        ++next_k;

        /* Safety: if we've already recorded all counts 0..4 we can stop,
           though this condition is unlikely to trigger before the
           requested n is found. */
        if (has_k[0] && has_k[1] && has_k[2] && has_k[3] && has_k[4]) {
            /* All possible counts have been seen; the requested n must
               have been found, otherwise it is impossible. */
            return (seq_t)0;
        }
    }
}
