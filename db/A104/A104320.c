/*
 * A104320  Number of zeros in ternary representation of 2^n.
 * https://oeis.org/A104320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of digits equal to 0 in the ternary representation
 *            of 2^n. The algorithm stores 2^n in base 3^19 (3^19 = 1 162 261
 *            467) and repeatedly doubles it. After the loop the base‑3 digits
 *            are extracted and zeros are counted. This works for arbitrary n
 *            and for any integer type seq_t
 * Range:     u8 n <= 1113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base 3^19 fits in a 32‑bit word. */
    const uint32_t BASE = 1162261467U;   /* 3^19 */
    /* Dynamic array of limbs. */
    size_t cap = 4;          /* initial capacity */
    size_t len = 1;          /* number of limbs currently used */
    uint32_t *limb = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!limb) return (seq_t)0;  /* allocation failed */

    limb[0] = 1;          /* start with 2^0 = 1 */

    /* Multiply by 2, n times. */
    for (idx_t step = 0; step < n; ++step) {
        uint64_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint64_t prod = (uint64_t)limb[i] * 2ULL + carry;
            limb[i] = (uint32_t)(prod % BASE);
            carry   = prod / BASE;
        }
        if (carry) {
            if (len >= cap) {                 /* grow array */
                cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(limb, cap * sizeof(uint32_t));
                if (!tmp) { free(limb); return (seq_t)0; }
                limb = tmp;
            }
            limb[len++] = (uint32_t)carry;
        }
    }

    /* Count zeros in the ternary representation. */
    seq_t zeros = (seq_t)0;
    if (len == 0) {                         /* should not happen */
        free(limb);
        return zeros;
    }

    /* Process the most significant limb specially (no leading zeros). */
    uint32_t top = limb[len - 1];
    while (top > 0) {
        if ((top % 3U) == 0U) ++zeros;
        top /= 3U;
    }

    /* Process all lower limbs; each contains exactly 19 base‑3 digits. */
    for (size_t i = 0; i + 1 < len; ++i) {
        uint32_t val = limb[i];
        for (int d = 0; d < 19; ++d) {      /* 19 digits per limb */
            if ((val % 3U) == 0U) ++zeros;
            val /= 3U;
        }
    }

    free(limb);
    return zeros;
}
