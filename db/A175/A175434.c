/*
 * A175434  (Digit sum of 2^n) mod n.
 * https://oeis.org/A175434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (digit sum of 2^n) mod n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Maximum number of base‑1e9 blocks required for 2^10000 (~3011 decimal digits) */
#define MAX_BLOCKS 400

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined, but return 0 safely */

    static uint32_t digits[MAX_BLOCKS];
    size_t len = 1;
    digits[0] = 1u;                /* 2^0 = 1 */

    /* compute 2^n in base 1e9 */
    for (idx_t i = 0; i < n; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint64_t cur = (uint64_t)digits[j] * 2u + carry;
            digits[j] = (uint32_t)(cur % 1000000000u);
            carry    = cur / 1000000000u;
        }
        if (carry) {
            digits[len++] = (uint32_t)carry;   /* carry is 1 for base 1e9 */
        }
    }

    /* sum decimal digits of the representation */
    uint64_t sum = 0;
    for (size_t j = 0; j < len; ++j) {
        uint32_t v = digits[j];
        while (v > 0) {
            sum += v % 10u;
            v   /= 10u;
        }
    }

    seq_t res = (seq_t)(sum % (uint64_t)n);
    return res;
}
