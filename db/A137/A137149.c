/*
 * A137149  a(n) = (prime(n)-2)!.
 * https://oeis.org/A137149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)-2)!
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   factorial, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint32_t p = get_prime(n);
    idx_t m = (idx_t)(p - 2);
    return factorial(m);
}
