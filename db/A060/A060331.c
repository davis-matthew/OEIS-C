/*
 * A060331  Primes the sum of six consecutive composite numbers.
 * https://oeis.org/A060331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 391, u32 n <= 440, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth prime that can be written as the sum of six consecutive
   composite numbers.  The algorithm walks through the integers, keeps
   a sliding window of the last six composites, and tests whether the
   window sum is prime.  The first prime found corresponds to a(1). */
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Offset 1: return value of a(1) is the first prime found. */
    if (n == 0) return 0;               /* behaviour below offset is irrelevant */

    seq_t window[6] = {0};              /* last six composites (as seq_t)   */
    seq_t sum = 0;                      /* sum of the current window        */
    idx_t count = 0;                    /* number of composites seen */
    idx_t found = 0;                    /* how many prime sums found */

    idx_t m = 4;                        /* start checking from 4            */

    while (1) {
        /* Skip primes; composite numbers are 4 or more and not prime. */
        if (m >= 4 && !is_prime(m)) {
            seq_t val = (seq_t)m;

            if (count < 6) {            /* filling the initial window   */
                window[count++] = val;
                sum += val;

                if (count == 6 && is_prime_big(sum)) {
                    ++found;
                    if (found == n) return sum;
                }
            } else {
                /* Slide window: drop the oldest composite. */
                sum -= window[0];
                for (idx_t i = 0; i < 5; ++i) window[i] = window[i + 1];
                window[5] = val;
                sum += val;

                if (is_prime_big(sum)) {
                    ++found;
                    if (found == n) return sum;
                }
            }
        }
        ++m;
    }
}
