/*
 * A081445  Smallest squares such that partial sums of the sequence plus 11
 *          are primes.
 * https://oeis.org/A081445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 240, u32 n <= 240, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A081445:  smallest squares (6m)^2 such that partial sums + 11 are primes.
   a(1) is the first term (36), a(2) = 36, a(3) = 144, …                   */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    seq_t sum   = 0;                      /* running sum of the sequence */
    seq_t value = 0;                      /* term a(n) to return */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t candidate = 0;
        idx_t m = 1;

        /* search for smallest m with (6m)^2 making sum+11 prime */
        while (1) {
            seq_t mm   = (seq_t)m;
            seq_t sq   = (seq_t)36 * mm * mm;   /* (6m)^2 */
            seq_t test = sum + sq + (seq_t)11;
            if (is_prime_big(test)) {
                candidate = sq;
                break;
            }
            ++m;
        }

        /* update sum and, if last iteration, store result */
        sum += candidate;
        if (i == n) value = candidate;
    }
    return value;
}
