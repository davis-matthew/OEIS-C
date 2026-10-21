/*
 * A061867  Squares whose product of digits is also a square (allowing zeros).
 * https://oeis.org/A061867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the nth square whose decimal‑digit product is a perfect
 *            square (a product of zero is accepted)
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It searches successive squares, counting those that satisfy the property. */

#include <stdint.h>
#include <stdbool.h>

/* Parity masks for the prime factors 2,3,5,7 of each digit.
   A digit contributes 1 to the parity of a prime if the exponent
   of that prime in the digit is odd.  The mask is a 4‑bit value:
   bit 3 – parity of 2, bit 2 – parity of 3, bit 1 – parity of 5,
   bit 0 – parity of 7.  The resulting mask is zero iff the
   product of the digits is a square. */
static const unsigned char digit_mask[10] = {
    0,          /* 0 – handled separately */
    0,          /* 1 -> 2^0 3^0 5^0 7^0  -> 0000 */
    0x8,        /* 2 -> 2^1            -> 1000 */
    0x4,        /* 3 -> 3^1            -> 0100 */
    0,          /* 4 -> 2^2            -> 0000 */
    0x2,        /* 5 -> 5^1            -> 0010 */
    0xC,        /* 6 -> 2^1 3^1        -> 1100 */
    0x1,        /* 7 -> 7^1            -> 0001 */
    0x8,        /* 8 -> 2^3            -> 1000 */
    0           /* 9 -> 3^2            -> 0000 */
};

seq_t a(idx_t n)
{
    if (n == 0)   /* the sequence is 1‑based; a(0) is undefined,
                      but we return 0 to avoid UB. */
        return 0;

    idx_t count = 0;
    idx_t k = 0;

    while (true) {
        /* compute k² without relying on overflow of the target type
           (k is up to 4.3 e9 for a 64‑bit target type). */
        unsigned long long sq = (unsigned long long)k * (unsigned long long)k;

        /* Determine whether the square satisfies the property. */
        bool has_zero = false;
        unsigned mask = 0;

        unsigned long long t = sq;
        if (t == 0) {           /* square of 0 */
            has_zero = true;
        } else {
            while (t > 0) {
                unsigned d = t % 10;
                t /= 10;
                if (d == 0) {
                    has_zero = true;
                    break;
                }
                mask ^= digit_mask[d];
            }
        }

        if (has_zero || mask == 0) {
            ++count;
            if (count == n)
                return (seq_t)sq;   /* exact if it fits in seq_t */
        }
        ++k;
    }
}
