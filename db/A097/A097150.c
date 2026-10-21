/*
 * A097150  Digits of balanced base-10 representations of nonnegative integers
 *          (most significant digits first).
 * https://oeis.org/A097150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 112, i16 n <= 112, i32 n <= 112, i64 n <= 112,
 *            i128 n <= 112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) as the nth digit of the concatenated balanced
    base‑10 representations of nonnegative integers (most significant
    digits first).  The digits allowed are in {-5,-4,-3,-2,-1,0,1,2,3,4}.
    For each integer i ≥ 0 we write i in balanced base‑10 and then
    output its digits from the most significant to the least
    significant.  The sequence a(n) is the infinite stream of these
    digits, indexed starting at 1. */

#include <stddef.h>   /* for size_t */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* buffer for the digits of the current integer, least significant first */
    seq_t digits[32];                     /* max digits for i up to 2^64 is < 32 */
    idx_t dcnt;
    idx_t cumul = 0;                      /* digits produced so far */

    for (idx_t i = 0;; ++i) {
        /* compute digits of integer i */
        if (i == 0) {
            digits[0] = (seq_t)0;
            dcnt = 1;
        } else {
            int64_t xi = (int64_t)i;      /* signed copy for the algorithm */
            dcnt = 0;
            while (xi != 0) {
                int r = (int)(xi % 10);   /* remainder 0..9 */
                if (r > 4) r -= 10;       /* convert to -5..4 */
                digits[dcnt++] = (seq_t)r;/* store least significant first */
                xi = (xi - r) / 10;       /* next higher digit */
            }
        }

        /* does the desired term lie within this integer's digits? */
        if (cumul + dcnt >= n) {
            idx_t offset = n - cumul - 1;                 /* zero‑based index in this block */
            idx_t idx = dcnt - 1 - offset;                /* digits[ ] stored LSF first */
            return digits[idx];
        }
        cumul += dcnt;                                   /* advance cumulative count */
    }
}
