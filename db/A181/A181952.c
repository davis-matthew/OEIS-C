/*
 * A181952  n!, digits ordered, zeros omitted.
 * https://oeis.org/A181952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = digits of n! sorted ascending, zeros omitted
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 16, u64 n <= 28, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* big‑integer in base 1e9 stored in an array of uint32_t */
    const uint32_t BASE = 1000000000U;
    size_t cap = 1;          /* current allocated size (limbs) */
    size_t len = 1;          /* number of used limbs */
    uint32_t *limbs = (uint32_t*)malloc(sizeof(uint32_t) * cap);
    if (!limbs) return (seq_t)0;    /* OOM – unlikely in tests */

    limbs[0] = 1;            /* 0! = 1 */

    /* factorial: multiply by each integer i = 2 .. n */
    for (idx_t i = 2; i <= n; ++i) {
        if (len == cap) {    /* grow storage */
            cap <<= 1;
            uint32_t *tmp = (uint32_t*)realloc(limbs, sizeof(uint32_t) * cap);
            if (!tmp) { free(limbs); return (seq_t)0; }
            limbs = tmp;
        }

        unsigned long long carry = 0;
        uint32_t mult = (uint32_t)i;
        for (size_t j = 0; j < len; ++j) {
            unsigned long long prod = (unsigned long long)limbs[j] * mult + carry;
            limbs[j] = (uint32_t)(prod % BASE);
            carry    = prod / BASE;
        }
        while (carry) {
            limbs[len++] = (uint32_t)(carry % BASE);
            carry       /= BASE;
        }
    }

    /* count decimal digits of the factorial (excluding leading zeros) */
    unsigned int counts[10] = {0};

    /* process most significant limb (no leading zeros) */
    size_t msd_index = len - 1;
    uint32_t msd = limbs[msd_index];
    if (msd != 0) {
        while (msd > 0) {
            ++counts[msd % 10];
            msd /= 10;
        }
    }

    /* process remaining limbs (each contributes exactly 9 decimal digits) */
    for (size_t idx = 0; idx < msd_index; ++idx) {
        uint32_t limb = limbs[idx];
        for (int d = 0; d < 9; ++d) {
            ++counts[limb % 10];
            limb /= 10;
        }
    }

    /* assemble result: digits 1‑9 in ascending order, zeros omitted */
    seq_t result = (seq_t)0;
    for (int d = 1; d <= 9; ++d) {
        for (size_t c = 0; c < counts[d]; ++c) {
            result = result * (seq_t)10 + (seq_t)d;
        }
    }

    free(limbs);
    return result;
}
