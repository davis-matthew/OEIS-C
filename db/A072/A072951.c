/*
 * A072951  a(n) = Sum_{k=1..n} binomial(k, n mod k).
 * https://oeis.org/A072951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(k, n mod k)
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   multiplicative_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/multiplicative_binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t r = n % k;
        sum += multiplicative_binom(k, r);
    }
    return sum;
}
