/*
 * A115454  Composite positive integers written in base 2.
 * https://oeis.org/A115454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A007088(A002808(n)) // binary representation of the nth
 *            composite number
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 850, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               // Undefined for offset 0

    idx_t count   = 0;                   // how many composites seen
    idx_t num     = 4;                   // first composite (2*2)
    idx_t comp    = 0;                   // value of the nth composite

    while (count < n) {
        if (!is_prime(num)) {
            ++count;
            if (count == n) {
                comp = num;
                break;
            }
        }
        ++num;
    }

    /* Convert comp to its binary digits interpreted as a decimal integer */
    idx_t msb = 0;
    idx_t t   = comp;
    while (t >>= 1) msb++;               // find position of highest set bit

    seq_t result = 0;
    for (int i = (int)msb; i >= 0; --i) {
        seq_t bit = (comp >> i) & 1;
        result = result * 10 + bit;
    }
    return result;
}
