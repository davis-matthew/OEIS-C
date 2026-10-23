/*
 * A100800  Let f(n) = n + sum of the digits of n. If f(n) is multiple of n
 *          then a(n)= f(n) else a(n) = f(f(f(n)))... until one gets a
 *          multiple of n; a(n) = 0 if no such number exists.
 * https://oeis.org/A100800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the first iterated value f^k(n), starting with f(n), *
 *            such that f^k(n) is divisible by n; returns 0 if none * is found
 *            within a reasonable bound
 * Range:     u8 n <= 10, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    seq_t mod = (seq_t)n;
    seq_t cur = mod + digit_sum(mod);          /* start with f(n)      */
    for (uint64_t i = 0; i < 1000000; ++i) {    /* generous safety loop */
        if (cur % mod == 0)
            return cur;
        cur += digit_sum(cur);
    }
    return 0;                                   /* should not happen   */
}
