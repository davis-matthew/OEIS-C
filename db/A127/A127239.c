/*
 * A127239  Central coefficients of Thue-Morse binomial triangle A127236.
 * https://oeis.org/A127239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A010060(binomial(2n,n)) – parity of the number of 1‐bits
 *            in the central binomial coefficient C(2n,n). The value is 0 or 1
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

/* Return 0 or 1 – the parity of the number of 1-bits in C(2n,n).  */
seq_t a(idx_t n)
{
    /* Special case n = 0: C(0,0) = 1, popcount = 1 → parity 1.  */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate a big‑integer buffer large enough for C(2n,n).  */
    size_t limbs_needed = (2 * n + 32) / 32 + 2;  /* +2 for safety */
    uint32_t *limbs = (uint32_t *)malloc(limbs_needed * sizeof(uint32_t));
    if (!limbs) return (seq_t)0;  /* Out‑of‑memory; return 0 as safe fallback */

    size_t len = 1;          /* current number of limbs used (least‑significant first) */
    limbs[0] = 1;           /* initialise to 1 */

    for (idx_t i = 1; i <= n; ++i) {
        /* Multiplies the big integer by (n + i).  */
        uint32_t mul = (uint32_t)(n + i);
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint64_t prod = (uint64_t)limbs[j] * mul + carry;
            limbs[j] = (uint32_t)prod;
            carry    = prod >> 32;
        }
        if (carry) {
            limbs[len++] = (uint32_t)carry;
            /* carry >> 32 is always zero because mul < 2^15; no further carry */
        }

        /* Divides the big integer by i.  The division is exact.  */
        uint64_t rem = 0;
        for (size_t j = len; j > 0; --j) {
            uint64_t cur = (rem << 32) | limbs[j - 1];
            uint32_t q  = (uint32_t)(cur / (uint32_t)i);
            rem        = cur % (uint32_t)i;
            limbs[j - 1] = q;
        }
        assert(rem == 0);  /* should divide evenly */

        /* Remove any leading zero limbs.  */
        while (len > 1 && limbs[len - 1] == 0) {
            --len;
        }
    }

    /* Compute parity of popcount of the big integer.  */
    uint8_t parity = 0;
    for (size_t j = 0; j < len; ++j) {
        parity ^= (__builtin_popcount(limbs[j]) & 1);
    }

    free(limbs);
    return (seq_t)parity;
}
