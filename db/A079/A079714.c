/*
 * A079714  Number of 2's in n!.
 * https://oeis.org/A079714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = count of digit '2' in the decimal expansion of n!
 * Range:     u8 n <= 965, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A079714: Number of 2's in the decimal representation of n! */
/* Implementation: compute n! with big‑integer blocks (base 1e9) and count
 * the digit 2 in the resulting representation.  This works for any n
 * whose factorial fits into memory, and the count easily fits in seq_t. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* base of the big integer (fits in 32 bits) */
    const uint32_t BASE = 1000000000u;   /* 1e9 */

    /* allocate digits array dynamically */
    size_t cap  = 16;        /* initial capacity */
    size_t len  = 1;         /* number of used blocks (least significant first) */
    uint32_t *digits = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!digits) return 0;   /* unlikely but keep compiler happy */
    digits[0] = 1;           /* 0! = 1 */

    /* build n! */
    for (idx_t i = 2; i <= n; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint64_t prod = (uint64_t)digits[j] * (uint64_t)i + carry;
            digits[j] = (uint32_t)(prod % BASE);
            carry     = prod / BASE;
        }
        while (carry) {
            if (len == cap) {   /* grow */
                cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(digits, cap * sizeof(uint32_t));
                if (!tmp) { free(digits); return 0; }
                digits = tmp;
            }
            digits[len++] = (uint32_t)(carry % BASE);
            carry /= BASE;
        }
    }

    /* count digit '2' in the decimal representation */
    seq_t count = 0;
    for (size_t k = 0; k < len; ++k) {
        uint32_t v = digits[k];
        if (k == len - 1) {   /* most significant block, no leading zeros */
            if (v == 0) {
                /* factorial 0 or 1 gives value 1, handled below */
                count += 0;
            } else {
                while (v > 0) {
                    if ((v % 10) == 2) ++count;
                    v /= 10;
                }
            }
        } else {              /* all blocks are exactly 9 digits (with leading zeros) */
            for (int d = 0; d < 9; ++d) {
                if ((v % 10) == 2) ++count;
                v /= 10;
            }
        }
    }

    free(digits);
    return count;
}
