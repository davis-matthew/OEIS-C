/*
 * A095549  a(n) = 78 written in base 15 - n.
 * https://oeis.org/A095549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal value of the base (15 – n) representation of the
 *            * decimal number 78. For positive bases the usual base
 *            conversion is used. * For base 1 a unary (78 ones) is produced.
 *            For negative bases with |b|>1 we * use the standard algorithm
 *            for negative base representation. For base 0 * (n =...
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: full, pass, 15 terms through n = 14 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* helper that converts the integer 78 into base `base` and returns the
 * decimal interpretation of the digit string. Works for base > 1, base == 1
 * (unary) and negative bases with |base| > 1. */
static seq_t convert_base(int64_t value, int64_t base)
{
    /* Unary base: 78 -> "111...1" (78 times) */
    if (base == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < (idx_t)value; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* Positive base > 1 */
    if (base > 1) {
        int digits[64];
        int cnt = 0;
        int64_t n = value;
        while (n > 0) {
            digits[cnt++] = (int)(n % base);
            n /= base;
        }
        if (cnt == 0) {
            digits[cnt++] = 0;
        }
        seq_t res = 0;
        for (int i = cnt - 1; i >= 0; --i)
            res = res * 10 + (seq_t)digits[i];
        return res;
    }

    /* Negative base |base| > 1 */
    if (base < -1) {
        int digits[64];
        int cnt = 0;
        int64_t n = value;
        int64_t abs_base = -base;
        while (n != 0) {
            int64_t rem = n % abs_base;
            if (rem < 0)
                rem += abs_base;
            digits[cnt++] = (int)rem;
            n = (n - rem) / base;  /* base is negative */
        }
        if (cnt == 0) {
            digits[cnt++] = 0;
        }
        seq_t res = 0;
        for (int i = cnt - 1; i >= 0; --i)
            res = res * 10 + (seq_t)digits[i];
        return res;
    }

    /* base == 0 or base == -1 : undefined for this sequence */
    return 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Compute base = 15 – n as a signed integer */
    int64_t base = (int64_t)15 - (int64_t)n;
    if (base == 0)   /* n == 15, invalid base */
        return 0;
    return convert_base((int64_t)78, base);
}
