/*
 * A095567  a(n) = 87 written in base 10 - n.
 * https://oeis.org/A095567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal integer corresponding to the base-(10 - n)
 *            representation of 87. For base 1 (n = 9) the representation is
 *            unary (87 ones). For base ≤ 0 the value is undefined and the
 *            function returns 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the base (10 - n) as a signed 64‑bit integer. */
    int64_t base = 10LL - (int64_t)n;

    /* Base ≤ 0 : undefined in this context, return 0. */
    if (base <= 0) {
        return (seq_t)0;
    }

    /* Unary representation (base 1). */
    if (base == 1) {
        seq_t res = 0;
        for (uint64_t i = 0; i < 87; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* Positive base (>=2). */
    if (base >= 2) {
        uint64_t val = 87ULL;
        uint16_t digits[64];   /* enough for any length here. */
        int   pos   = 0;

        while (val > 0) {
            uint16_t d = val % (uint64_t)base;
            digits[pos++] = d;
            val /= (uint64_t)base;
        }

        seq_t res = 0;
        for (int i = pos - 1; i >= 0; --i) {
            res = res * 10 + digits[i];
        }
        return res;
    }

    /* Negative base (<= -2).  Use the standard negative‑base conversion algorithm. */
    /* base is negative.  abs_base is its absolute value. */
    int64_t abs_base = -base;  /* base is negative, so abs_base > 0. */
    int64_t val = 87LL;
    uint16_t digits[64];
    int pos = 0;

    while (val != 0) {
        int64_t r = val % abs_base;
        if (r < 0) r += abs_base;          /* ensure a non‑negative remainder. */
        val = (val - r) / base;            /* base is negative. */
        digits[pos++] = (uint16_t)r;
    }

    seq_t res = 0;
    for (int i = pos - 1; i >= 0; --i) {
        res = res * 10 + digits[i];
    }
    return res;
}
