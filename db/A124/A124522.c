/*
 * A124522  a(n) = smallest k such that 2nk-1 and 2nk+1 are primes.
 * https://oeis.org/A124522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that 2*n*k-1 and 2*n*k+1 are primes
 * Range:     u8 n <= 12, u16 n <= 2163, u32 n <= 2503, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not part of OEIS sequence, safety */
    seq_t nval = (seq_t)n;          /* promote index to seq_t */

    for (idx_t k = 1; ; ++k) {
        seq_t kval = (seq_t)k;      /* promote loop counter */
        seq_t prod = 2 * nval * kval;  /* compute 2*n*k */

        seq_t v1 = prod - 1;       /* 2*n*k - 1 */
        seq_t v2 = prod + 1;       /* 2*n*k + 1 */

        if (is_prime_big(v1) && is_prime_big(v2))
            return kval;           /* found the smallest k */
    }
}
