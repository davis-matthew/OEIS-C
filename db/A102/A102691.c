/*
 * A102691  Least n-expodigital number (i.e., numbers m such that m^n has
 *          exactly n decimal digits).
 * https://oeis.org/A102691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   a(n) = 10 - (number of digits m in 0..9 such that m^n has
 *            exactly n decimal digits)
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#if defined(__cplusplus)
static inline void mul_small(uint32_t *digits, size_t *len, uint32_t mul) {
#else
static void mul_small(uint32_t *digits, size_t *len, uint32_t mul) {
#endif
    if (mul <= 1) return;          /* nothing to do */
    uint64_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        uint64_t prod = (uint64_t)digits[i] * mul + carry;
        digits[i] = (uint32_t)(prod % 1000000000U);
        carry = prod / 1000000000U;
    }
    if (carry) {
        digits[*len] = (uint32_t)carry;
        (*len)++;
    }
}

static inline int digit_len32(uint32_t x) {
    if (x >= 100000000U) return 9;
    if (x >= 10000000U)  return 8;
    if (x >= 1000000U)   return 7;
    if (x >= 100000U)    return 6;
    if (x >= 10000U)     return 5;
    if (x >= 1000U)      return 4;
    if (x >= 100U)       return 3;
    if (x >= 10U)        return 2;
    return 1;
}

seq_t a(idx_t n) {
    /* Buffer size sufficient for 9^10000 (~9500 decimal digits ⇒ ~1056 base1e9 limbs) */
    enum { BENCH_MAX_LIMBS = 1200 };
    typedef struct {
        size_t len;
        uint32_t d[BENCH_MAX_LIMBS];
    } BigInt;

    BigInt big[10];
    /* Initialise each big integer with m^1 */
    for (int i = 0; i <= 9; ++i) {
        if (i == 0) {
            big[i].len = 1;
            big[i].d[0] = 0;
        } else {
            big[i].len = 1;
            big[i].d[0] = (uint32_t)i;
        }
    }

    /* Exponentiation: compute m^n for each m = 1..9 */
    for (idx_t e = 1; e < n; ++e) {
        for (int m = 1; m <= 9; ++m) {
            mul_small(big[m].d, &big[m].len, (uint32_t)m);
        }
    }

    /* Count how many m yield n decimal digits */
    unsigned count = 0;
    for (int m = 0; m <= 9; ++m) {
        size_t digits;
        if (m == 0) {
            digits = 1;                     /* 0^n = 0 has one digit */
        } else {
            size_t len = big[m].len;
            uint32_t msd = big[m].d[len - 1];
            digits = (len - 1) * 9 + digit_len32(msd);
        }
        if (digits == (size_t)n) ++count;
    }

    return (seq_t)(10u - count);
}
