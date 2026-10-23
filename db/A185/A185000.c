/*
 * A185000  Trajectory of x+1 under the map (see A185544) defined in the
 *          Comments.
 * https://oeis.org/A185000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A185000 (trajectory of x+1
 * under the map described in A185544).  Starting with the
 * polynomial f = x + 1, successive terms are obtained by
 * repeatedly applying the GF(2)[x] map:
 *
 *   if f(0) == 0   →   f := f / x
 *   else           →   f := ((x² + 1) * f + 1) / x
 *
 * The integer returned is the decimal number whose decimal
 * digits are the binary coefficients of the final polynomial
 * (highest degree first).  The function works for any `n ≥ 1`
 * and uses only the operations permitted on `seq_t`. */

#include <stdint.h>
#include <stddef.h>

extern seq_t a(idx_t n)
{
    /* Fixed buffer: the sequence stabilises to a period of 4,
       so 8 coefficient slots are more than enough. */
    uint8_t buf[16] = {0};
    size_t len = 2;              /* start with x + 1 → [1, 1]       */
    buf[0] = 1;                  /* constant term */
    buf[1] = 1;                  /* coefficient of x */

    /* Apply the map n‑1 times to reach the polynomial of
       order a(n). */
    for (idx_t step = 1; step < n; ++step) {
        if (buf[0] == 0) {
            /* f(0) == 0   →   f := f / x  (shrink right) */
            for (size_t i = 0; i + 1 < len; ++i)
                buf[i] = buf[i + 1];
            --len;
        } else {
            /* f(0) == 1   →   f := ((x² + 1) * f + 1) / x */
            uint8_t tmp[16] = {0};
            size_t tlen = len + 2;          /* multiplication adds 2 */

            /* (x² + 1) * f  (GF(2) convolution) */
            for (size_t i = 0; i < len; ++i) {
                if (buf[i]) {
                    tmp[i] ^= 1;           /* coefficient for xᶦ            */
                    tmp[i + 2] ^= 1;       /* coefficient for xᶦ⁺²          */
                }
            }

            /* add 1 to constant term */
            tmp[0] ^= 1;

            /* divide by x: shift right by one */
            for (size_t i = 0; i + 1 < tlen; ++i)
                buf[i] = tmp[i + 1];

            len = tlen - 1;

            /* remove trailing zero coefficients */
            while (len > 0 && buf[len - 1] == 0)
                --len;
        }
    }

    /* Convert the coefficient vector into the desired decimal integer. */
    seq_t value = 0;
    for (long long i = (long long)len - 1; i >= 0; --i)
        value = value * 10 + buf[i];

    return value;
}
