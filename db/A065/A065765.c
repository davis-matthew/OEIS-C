/*
 * A065765  Sum of divisors of twice square numbers.
 * https://oeis.org/A065765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(2*n^2) = A000203(2*A000290(n))
 * Range:     u8 n <= 8, u16 n <= 104, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* for safety; OEIS offset starts at 1 */
    uint64_t m = 2ULL * n * n;   /* 2*n^2 fits in 64‑bit for all n in tests */
    return sigma_of((idx_t)m);
}
