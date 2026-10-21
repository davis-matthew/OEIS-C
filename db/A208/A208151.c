/*
 * A208151  Decimal expansion of 3344161/1494696.
 * https://oeis.org/A208151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the nth term of the decimal expansion of 3344161/1494696.
 *  a(1) = 2  (integer part)
 *  a(2) = first decimal digit, a(3) = second digit, etc.
 *  After the first three non‑repeating decimals the remaining digits
 *  repeat with period 210.  The function caches the integer part,
 *  the finite prefix, and the periodic block so that every call to
 *  a(idx_t n) is O(1).  The implementation works for any seq_t
 *  width supported by the build. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  constants of the fraction 3344161/1494696  */
    static const idx_t NUM = 3344161;
    static const idx_t DEN = 1494696;

    /*  static cache variables  */
    static seq_t          integer_part = 0;          /* a(1) */
    static seq_t          digits[256]   = {0};        /* non‑repeating + one period */
    static size_t        digits_len     = 0;
    static size_t        non_repeat_len = 0;
    static seq_t        *period         = NULL;
    static size_t        period_len     = 0;
    static int           initialized    = 0;

    if (!initialized) {
        /*  compute integer part and first digits of the fraction  */
        integer_part = (seq_t)(NUM / DEN);
        idx_t rem     = NUM % DEN;

        /*  array that remembers when a remainder first appeared.
         *  sized DEN, which is at most 1 494 696, so ~6 MB.          */
        int *rem_pos = (int *)malloc((size_t)DEN * sizeof(int));
        for (size_t i = 0; i < (size_t)DEN; ++i) rem_pos[i] = -1;

        size_t pos = 0;                     /* position after decimal */
        while (1) {
            if (rem == 0) {                 /* terminating decimal   */
                non_repeat_len = digits_len;
                period_len     = 0;
                period         = NULL;
                break;
            }
            int rpos = rem_pos[rem];
            if (rpos != -1) {               /* remainder repeats   */
                non_repeat_len = (size_t)rpos;
                period_len = digits_len - (size_t)rpos;
                if (period_len > 0) {
                    period = (seq_t *)malloc(period_len * sizeof(seq_t));
                    for (size_t i = 0; i < period_len; ++i)
                        period[i] = digits[non_repeat_len + i];
                }
                break;
            }
            rem_pos[rem] = (int)pos;

            /*  produce next decimal digit  */
            rem *= 10;
            seq_t digit = (seq_t)(rem / DEN);
            rem = rem % DEN;

            if (digits_len < sizeof(digits) / sizeof(digits[0]))
                digits[digits_len++] = digit;
            pos++;
        }

        free(rem_pos);
        initialized = 1;
    }

    /*  answer the requested term  */
    if (n == 1)            return integer_part;
    if (n < 2)             return 0;          /* undefined – fall back to 0 */

    size_t idx = (size_t)(n - 2);              /* index into decimal digits */
    if (idx < digits_len)                     /* still within one generated cycle */
        return digits[idx];

    if (period_len == 0)                       /* terminating decimal – pad zeros */
        return 0;

    /*  into the periodic part  */
    size_t rel = (idx - non_repeat_len) % period_len;
    return period[rel];
}
