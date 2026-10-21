/*
 * A082466  Least k>=1 such that n^2+kn-1 and n^2+kn+1 are twin primes.
 * https://oeis.org/A082466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 1 such that n^2 + k*n - 1 and n^2 + k*n + 1
 *            are twin primes
 * Range:     u8 n <= 6, u16 n <= 217, u32 n <= 291, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Computes the least k >= 1 with n^2 + k*n - 1 and n^2 + k*n + 1 both prime. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n = 0 – return 0.  Calls with n >= 1 are required. */
        return 0;
    }

    seq_t nsq = (seq_t)n * (seq_t)n;   /* n^2, computed once */
    seq_t k = 1;                       /* start with the smallest admissible k */

    for (;;) {
        seq_t x   = nsq + (seq_t)n * k; /* n^2 + k*n  */
        seq_t lo  = x - 1;             /* n^2 + k*n -1 */
        seq_t hi  = x + 1;             /* n^2 + k*n +1 */

        if (is_prime_big(lo) && is_prime_big(hi))
            return k;

        ++k;   /* try the next candidate k */
    }
}
