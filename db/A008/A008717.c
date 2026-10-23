/*
 * A008717  20 in base 20-n.
 * https://oeis.org/A008717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, base, full
 * Formula:   a(n) = representation of decimal 20 in base (20-n) as a decimal
 *            integer
 * Range:     u8 n <= 17, u16 n <= 18, u32 n <= 18, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* See OEIS A008717 – “20 in base 20‑n”. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* valid only for n < 20; beyond that return 0 (undefined). */
    if (n >= 20u)
        return 0;

    /* base (20-n), will be between 1 and 20 inclusive */
    seq_t base = (seq_t)(20u - n);

    /* Special case: unary base */
    if (base == 1) {
        seq_t r = 0;
        for (idx_t i = 0; i < 20u; ++i)
            r = r * 10 + 1;
        return r;
    }

    /* Convert the decimal number 20 to the given base and build its decimal
       representation.  The digits of the representation are all < 10, so
       they can be stored as 8‑bit integers. */
    seq_t num = 20;
    uint8_t digits[32] = {0};   /* enough for any possible length */
    idx_t cnt = 0;

    while (num > 0) {
        seq_t rem = num % base;   /* remainder < base <= 20 < 256 */
        num = num / base;
        digits[cnt++] = (uint8_t)rem;
    }

    seq_t result = 0;
    while (cnt > 0) {
        result = result * 10 + digits[--cnt];
    }
    return result;
}
