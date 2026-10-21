/*
 * A065714  Number of 3's in decimal expansion of 2^n.
 * https://oeis.org/A065714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of decimal digit '3' in 2^n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Count of 3's in decimal representation of 2^n.
   Computation uses a manual base‑10^9 big integer to avoid overflow
   for any implementation where seq_t is an ordinary unsigned integer.
   The algorithm runs in O(n * log10(2^n)) ≈ O(n * 0.3) operations,
   comfortably below the time limit for n ≤ 10 000. */
seq_t a(idx_t n)
{
    /* base 10^9 for efficient decimal handling */
    const uint32_t BASE = 1000000000u;

    /* Maximum number of 10^9 blocks for n=10 000 is ≈ 335,
       we allow some headroom. */
    static uint32_t digits[4000];
    size_t len = 1;            /* number of used blocks */
    digits[0] = 1;             /* 2^0 = 1 */

    /* Multiply by 2, n times. */
    for (idx_t i = 0; i < n; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint64_t tmp = (uint64_t)digits[j] * 2u + carry;
            digits[j] = (uint32_t)(tmp % BASE);
            carry     = tmp / BASE;
        }
        if (carry) {
            digits[len++] = (uint32_t)carry;
        }
    }

    /* Count digit '3' in the decimal representation. */
    seq_t count = 0;
    for (size_t j = 0; j < len; ++j) {
        uint32_t val = digits[j];
        if (j == len - 1) { /* most significant block – no leading zeros */
            if (val == 0) continue;
            while (val > 0) {
                if ((val % 10u) == 3u) ++count;
                val /= 10u;
            }
        } else {
            /* other blocks: always 9 decimal digits (may have leading zeros) */
            for (int k = 0; k < 9; ++k) {
                if ((val % 10u) == 3u) ++count;
                val /= 10u;
            }
        }
    }

    return (seq_t)count;
}
