/*
 * A061117  Maximum number of divisors for any composite between prime(n) and
 *          prime(n+1).
 * https://oeis.org/A061117
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum number of divisors of composites between prime(n)
 *            and prime(n+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n < 2)          /* undefined for n < 2; return 0 */
        return (seq_t)0;

    uint64_t p_n     = (uint64_t)get_prime(n);     /* prime(n)   */
    uint64_t p_next  = (uint64_t)get_prime(n + 1); /* prime(n+1) */

    seq_t max_d = (seq_t)0;

    for (uint64_t x = p_n + 1; x < p_next; ++x)
    {
        if (!is_prime(x))
        {
            seq_t d = (seq_t)divisor_count_u64(x);
            if (d > max_d)
                max_d = d;
        }
    }
    return max_d;
}
