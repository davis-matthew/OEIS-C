/*
 * A087773  Number of common decimal digits of n and 2^n.
 * https://oeis.org/A087773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct decimal digits that appear in both n
 *            and 2ⁿ
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Digit mask: bit d is set iff digit d occurs */
typedef uint16_t digit_mask_t;

/* Big integer stored in base 1 000 000 000 (fits in 32 bits) */
struct BigInt {
    uint32_t *d;   /* least‑significant limb first */
    size_t sz;     /* number of used limbs */
};

/* Multiply a BigInt by 2 */
static void bigint_mul2(struct BigInt *bi)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < bi->sz; ++i) {
        uint64_t prod = (uint64_t)bi->d[i] * 2 + carry;
        bi->d[i] = (uint32_t)(prod % 1000000000U);
        carry    = prod / 1000000000U;
    }
    if (carry) {
        bi->d = (uint32_t *)realloc(bi->d, (bi->sz + 1) * sizeof(uint32_t));
        bi->d[bi->sz++] = (uint32_t)carry;
    }
}

/* Return decimal digit mask of a BigInt */
static digit_mask_t digits_mask_bigint(const struct BigInt *bi)
{
    digit_mask_t mask = 0;
    for (size_t i = 0; i < bi->sz; ++i) {
        uint32_t limb = bi->d[i];
        if (i + 1 == bi->sz) {           /* most significant limb – real number of digits */
            if (limb == 0) {
                mask |= 1u << 0;
            } else {
                while (limb) {
                    mask |= 1u << (limb % 10);
                    limb /= 10;
                }
            }
        } else {                         /* lower limbs always 9 decimal digits (with leading zeros) */
            for (int k = 0; k < 9; ++k) {
                mask |= 1u << (limb % 10);
                limb /= 10;
            }
        }
    }
    return mask;
}

/* Return decimal digit mask of a small non‑negative integer */
static digit_mask_t digit_mask_uint(idx_t n)
{
    if (n == 0) return 1u << 0;
    digit_mask_t mask = 0;
    while (n > 0) {
        mask |= 1u << (n % 10);
        n /= 10;
    }
    return mask;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static int initialized = 0;
    static struct BigInt pow2 = {NULL, 0};
    static digit_mask_t *digitMaskPow = NULL;
    static seq_t *values = NULL;
    static idx_t lastComputed = 0;

    if (!initialized) {
        /* initialise 2⁰ = 1 */
        pow2.sz = 1;
        pow2.d  = (uint32_t *)malloc(sizeof(uint32_t));
        pow2.d[0] = 1U;

        /* allocate space for first value */
        values      = (seq_t *)malloc(sizeof(seq_t));
        digitMaskPow = (digit_mask_t *)malloc(sizeof(digit_mask_t));

        digitMaskPow[0] = digits_mask_bigint(&pow2);
        values[0] = (seq_t)__builtin_popcount((unsigned)(digitMaskPow[0] & digit_mask_uint(0)));
        lastComputed = 0;
        initialized = 1;
    }

    if (n > lastComputed) {
        idx_t newSize = n + 1;
        values      = (seq_t *)realloc(values, newSize * sizeof(seq_t));
        digitMaskPow = (digit_mask_t *)realloc(digitMaskPow, newSize * sizeof(digit_mask_t));

        for (idx_t i = lastComputed + 1; i <= n; ++i) {
            bigint_mul2(&pow2);
            digitMaskPow[i] = digits_mask_bigint(&pow2);
            int common = __builtin_popcount((unsigned)(digitMaskPow[i] & digit_mask_uint(i)));
            values[i] = (seq_t)common;
        }
        lastComputed = n;
    }

    return values[n];
}
