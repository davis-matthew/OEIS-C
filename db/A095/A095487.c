/*
 * A095487  a(n) = 47 written in base 11 - n.
 * https://oeis.org/A095487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = "47" written in base (11 – n) and interpreted in decimal.
 * For base = 1 the representation is 47 ones (unary).  For base <= 0 the
 * result is undefined; the function returns 0.
 */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* signed base value */
    int64_t base = 11 - (int64_t)n;

    /* base == 1  ->  unary representation */
    if (base == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 47; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* base <= 0  ->  undefined for this sequence */
    if (base <= 0)
        return 0;

    /* base >= 2  ->  normal base conversion */
    if (base >= 2) {
        uint8_t digits[64];
        size_t len = 0;
        int64_t num = 47;             /* work in signed to handle mod with base */

        /* convert 47 to base 'base', collecting least‑significant digits first */
        while (num != 0) {
            int64_t r = num % base;   /* r could be negative if base negative, but base>=2 here */
            if (r < 0) r += base;
            digits[len++] = (uint8_t)r;
            num = (num - r) / base;
        }

        /* build decimal number by concatenating digits in reverse order */
        seq_t res = 0;
        while (len > 0) {
            --len;
            res = res * 10 + digits[len];
        }
        return res;
    }

    /* base <= -2  ->  negative base representation */
    /* (negative base handling is analogous to positive base, but with sign adjustments) */
    if (base <= -2) {
        uint8_t digits[64];
        size_t len = 0;
        int64_t absb = -base;         /* absolute value of the base */
        int64_t num = 47;

        while (num != 0) {
            int64_t r = num % base;   /* remainder can be negative */
            if (r < 0) r += absb;     /* make it non‑negative */
            digits[len++] = (uint8_t)r;
            num = (num - r) / base;
        }

        seq_t res = 0;
        while (len > 0) {
            --len;
            res = res * 10 + digits[len];
        }
        return res;
    }

    /* Should not reach here */
    return 0;
}
