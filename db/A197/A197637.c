/*
 * A197637  Number of non-Wilson primes <= n.
 * https://oeis.org/A197637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = π(n) - (# of Wilson primes ≤ n)
 * Range:     u8 n <= 1636, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/compute_pi.h"

/* Known Wilson primes (currently 5, 13, 563).  The list is sorted. */
static const idx_t WILSON_PRIMES[] = {5, 13, 563};
static const size_t NUM_WILSON   = 3;

seq_t a(idx_t n)
{
    /* Count the number of Wilson primes up to n.  The array is
       sorted, so we can stop at the first element larger than n. */
    size_t w_cnt = 0;
    for (size_t i = 0; i < NUM_WILSON; ++i) {
        if (WILSON_PRIMES[i] <= n) {
            ++w_cnt;
        } else {
            break;
        }
    }

    /* Number of primes ≤ n. */
    seq_t prime_cnt = compute_pi(n);

    /* The sequence value is the number of primes minus the number
       of Wilson primes counted. */
    return prime_cnt - (seq_t)w_cnt;
}
