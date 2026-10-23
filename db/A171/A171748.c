/*
 * A171748  Primes of the form (2+n)*(1+2*n)+(1+n)*(2+2*n).
 * https://oeis.org/A171748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form 4*n^2 + 9*n + 4
 * Range:     u8 n <= 3, u16 n <= 34, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the helper function is_prime_small is provided by the utilities
   header "../utils/is_prime_small.h". */
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    seq_t value;
    for (idx_t k = 1; ; ++k)
    {
        seq_t k_seq = (seq_t)k;
        value = 4 * k_seq * k_seq + 9 * k_seq + 4;
        if (is_prime_small(value))
        {
            ++cnt;
            if (cnt == n)
                return value;
        }
    }
}
