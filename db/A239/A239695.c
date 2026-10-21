/*
 * A239695  Base 9 sum of digits of prime(n).
 * https://oeis.org/A239695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of base‑9 digits of the nth prime
 * Range:     u8 n <= 54, u16 n <= 7910, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    uint32_t p = get_prime(n);          /* nth prime as uint32_t */
    seq_t  v   = (seq_t)p;              /* convert to seq_t */
    seq_t  sum = 0;
    const seq_t nine = (seq_t)9;

    while (v > (seq_t)0) {
        sum += v % nine;
        v   /= nine;
    }
    return sum;
}
