/*
 * A105134  Numbers n such that 16n+9 is prime.
 * https://oeis.org/A105134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n such that 16*n+9 is prime
 * Range:     u8 n <= 5, u16 n <= 830, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for offset 1, return 0 to avoid hanging */
        return 0;

    idx_t count = 0;
    idx_t m = 1;
    while (1) {
        seq_t p = (seq_t)m * 16 + 9;  /* value to test for primality */
        if (is_prime((uint64_t)p)) {
            count++;
            if (count == n)
                return (seq_t)m;
        }
        m++;
    }
}
