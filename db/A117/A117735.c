/*
 * A117735  a(n) = n! - primorial(n).
 * https://oeis.org/A117735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - primorial(n)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t fact = (seq_t)1;
    seq_t primorial = (seq_t)1;

    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
        if (is_prime(i))
            primorial *= (seq_t)i;
    }

    return fact - primorial;
}
