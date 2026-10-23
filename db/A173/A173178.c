/*
 * A173178  Numbers k such that 2*k+3 is a prime of the form 3*A024893(m) + 2.
 * https://oeis.org/A173178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, uned
 * Formula:   a(n) = 3*A059325(n) + 1
 * Range:     u8 n <= 27, u16 n <= 3614, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th integer k such that 2*k+3 is a prime ≡ 2 (mod 3). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* http://oeis.org/A173178 satisfies a(n) = (p(n)-3)/2,
       where p(n) is the n-th prime ≡ 2 (mod 3). */
    if (n == 0)
        return (seq_t)0;   /* undefined for n = 0, harmless */

    idx_t count = 0;
    uint64_t p = 5;          /* first prime ≡2 (mod 3) */

    /* Iterate over all numbers ≡2 (mod 3). */
    while (count < n) {
        if (is_prime(p) && (p % 3 == 2)) {
            ++count;
            if (count == n) {
                /* (p-3)/2 fits in seq_t for the range of n used. */
                return ((seq_t)p - (seq_t)3) / (seq_t)2;
            }
        }
        p += 6;   /* next candidate ≡2 (mod 3) */
    }
    /* Should never reach here. */
    return (seq_t)0;
}
