/*
 * A049238  a(n) is the number of divisors of prime(n) - 2.
 * https://oeis.org/A049238
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of (prime(n)-2)
 * Range:     u8 n <= 8416, u16 n <= 8416, u32 n <= 8416, u64 n <= 8416,
 *            u128 n <= 8416, big any n
 * Helpers:   divisor_count_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n < 2u) return 0;              /* offset is 2, value for n<2 is unused */
    uint32_t p = get_prime(n);          /* nth prime, n≥1 */
    uint64_t v = (uint64_t)p - 2u;      /* prime(n) - 2   */
    return (seq_t)divisor_count_u64(v); /* number of divisors of v */
}
