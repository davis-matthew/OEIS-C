/*
 * A107309  Concatenation of twin primes in reverse order.
 * https://oeis.org/A107309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = concatenation of the larger and smaller prime of the nth
 *            twin prime pair, i.e. (p+2) followed by p
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   count_digits, isPrime64, pow10_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrime64.h"
#include "../utils/count_digits.h"
#include "../utils/pow10_idx.h"

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    uint64_t p = 3;               /* first twin prime is (3,5) */
    while (1) {
        if (isPrime64(p) && isPrime64(p + 2)) {
            if (cnt == n) {
                int d      = count_digits(p);
                seq_t pow10 = pow10_idx((idx_t)d);
                return (seq_t)(p + 2) * pow10 + (seq_t)p;
            }
            ++cnt;
        }
        p += 2;   /* skip even numbers */
    }
}
