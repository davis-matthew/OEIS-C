/*
 * A032710  Numbers k such that k prefixed by '1' and followed by '3' is
 *          prime.
 * https://oeis.org/A032710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 284, u32 n <= 241, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the n-th integer k such that the decimal number 1k3 is prime.
   The function iterates over k=0,1,2,..., constructs 1k3 and
   counts the primes.  When the count reaches n it returns that k. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t      k            = 0;          /* current candidate k   */
    idx_t      count        = 0;          /* how many primes seen  */
    seq_t      pow10        = (seq_t)100; /* 10^(digits_of_k+1)   */
    idx_t      next_thresh  = 10;         /* next threshold for pow10 */

    for (;; ++k) {
        /* Construct the number 1k3 : 1*pow10 + k*10 + 3 */
        seq_t val = pow10 + (seq_t)k * (seq_t)10 + (seq_t)3;

        if (is_prime_big(val)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }

        /* When k grows to a new number of digits, increase pow10 */
        if (k + 1 == next_thresh) {
            next_thresh *= 10;
            pow10 *= (seq_t)10;
        }
    }
}
