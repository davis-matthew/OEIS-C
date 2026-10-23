/*
 * A145255  a(n) = the smallest positive integer that is non-coprime to n and
 *          has the same number of 0's in its binary representation as n has.
 * https://oeis.org/A145255
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = the smallest positive integer m such that gcd(m,n) > 1
 *            and the number of 0‑bits in the binary representation of m
 *            equals that of n (without leading zeros)
 * Range:     u8 n <= 255, u16 n <= 9459, u32 n <= 9459, u64 n <= 9459,
 *            u128 n <= 9459, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Count the number of 0‑bits in the binary representation of x
   without leading zeros. */
static uint64_t zero_bits(uint64_t x)
{
    if (x == 0) return 1;          /* binary "0" has one zero bit */
    int bitlen = 64 - __builtin_clzll(x);   /* position of MSB + 1 */
    int pop = __builtin_popcountll(x);
    return (uint64_t)bitlen - (uint64_t)pop;
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* Sequence is defined only for n ≥ 2.  For smaller n we simply
       return 0.  This value is ignored by the test harness because
       the offset is 2. */
    if (n < 2) return (seq_t)0;

    uint64_t z = zero_bits((uint64_t)n);   /* zero count of n */

    /* Search for the smallest m > 0 with gcd(m,n) > 1
       and the same zero count.  Since m = n satisfies the
       conditions, the loop is guaranteed to terminate. */
    for (uint64_t m = 2; ; ++m) {
        if (zero_bits(m) != z) continue;
        if (gcd((seq_t)m, (seq_t)n) > 1) {
            return (seq_t)m;
        }
    }
}
