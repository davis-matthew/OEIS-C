/*
 * A126210  Number of 8's in decimal expansion of 8^n.
 * https://oeis.org/A126210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of digit 8 in decimal expansion of 8^n
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* big integer in base 1e9, least‑significant word first */
    #define BASE 1000000000u
    /* 8^10000 has at most 9031 decimal digits → 1005 words of 1e9 are enough */
    uint32_t words[2000];
    int len = 1;
    words[0] = 1;

    for (idx_t i = 0; i < n; ++i) {
        uint64_t carry = 0;
        for (int j = 0; j < len; ++j) {
            uint64_t prod = (uint64_t)words[j] * 8 + carry;
            words[j] = (uint32_t)(prod % BASE);
            carry = prod / BASE;
        }
        if (carry) {
            words[len++] = (uint32_t)carry;
        }
    }

    uint64_t count = 0;
    for (int i = 0; i < len; ++i) {
        uint32_t w = words[i];
        if (i == len - 1) { /* most significant word – no leading zeros */
            if (w == 0) continue;
            while (w > 0) {
                if ((w % 10) == 8) ++count;
                w /= 10;
            }
        } else {                 /* full 9‑digit block, including leading zeros */
            uint32_t temp = w;
            for (int d = 0; d < 9; ++d) {
                if ((temp % 10) == 8) ++count;
                temp /= 10;
            }
        }
    }

    return (seq_t)count;
}
