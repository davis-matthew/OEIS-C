/*
 * A161835  Numbers k whose largest divisor <= sqrt(k) is 5.
 * https://oeis.org/A161835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A161835: Numbers k whose largest divisor <= sqrt(k) is 5.
   For n > 13, a(n) = 5 * prime(n-4); the first 13 terms are listed explicitly. */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* First 13 terms, indexed 1..13.  first[0] is unused. */
    static const seq_t first[14] = {
        0,
        25, 30, 35, 40, 45, 50, 55, 65, 75, 85, 95, 115, 125
    };

    if (n <= 13)
        return first[n];

    /* n > 13: use the (n-4)-th prime (1‑based). */
    uint32_t p = get_prime(n - 4);
    return (seq_t)5 * (seq_t)p;
}
