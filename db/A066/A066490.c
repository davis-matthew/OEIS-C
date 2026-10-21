/*
 * A066490  Number of primes of the form 4m+3 that are <= n.
 * https://oeis.org/A066490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of primes of the form 4m+3 that are ≤ n
 * Range:     u8 n <= 3606, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 0, a(3) = 1, a(4) = 1, … */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* For n < 3 the answer is 0 */
    if (n < 3u) {
        return 0;
    }

    seq_t count = 0;
    /* Only numbers ≡ 3 (mod 4) can satisfy the condition */
    for (idx_t p = 3u; p <= n; p += 4u) {
        if (is_prime(p)) {
            ++count;
        }
    }
    return count;
}
