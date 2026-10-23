/*
 * A175512  (Digit sum of 7^n) mod n.
 * https://oeis.org/A175512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (digit_sum(7^n)) mod n
 * Range:     u8 n <= 61, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const uint32_t BASE = 1000000000U;  /* 10^9 base for limbs */

seq_t a(idx_t n)
{
    /* Special case: n must be >= 1, but no harm handling n==0 */
    if (n == 0) return 0;

    size_t size = 1;               /* number of used limbs */
    size_t cap  = 4;               /* allocated capacity */
    uint32_t *limbs = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!limbs) return 0;          /* allocation failed */
    limbs[0] = 1;                  /* start with 7^0 = 1 */

    for (idx_t step = 1; step <= n; ++step) {
        uint64_t carry = 0;
        for (size_t i = 0; i < size; ++i) {
            uint64_t prod = (uint64_t)limbs[i] * 7 + carry;
            limbs[i] = (uint32_t)(prod % BASE);
            carry = prod / BASE;
        }
        if (carry) {
            if (size == cap) {
                cap <<= 1;
               	uint32_t *tmp = (uint32_t *)realloc(limbs, cap * sizeof(uint32_t));
                if (!tmp) { free(limbs); return 0; }
                limbs = tmp;
            }
            limbs[size++] = (uint32_t)carry;
        }
    }

    /* Compute digit sum of the resulting number */
    seq_t sum = 0;
    for (size_t i = 0; i < size; ++i) {
        uint32_t x = limbs[i];
        while (x) {
            sum += (seq_t)(x % 10);
            x /= 10;
        }
    }

    free(limbs);
    return sum % (seq_t)n;
}
