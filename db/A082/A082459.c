/*
 * A082459  Multiply by 1, add 1, multiply by 2, add 2, etc.
 * https://oeis.org/A082459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 14, i32 n <= 24, i64 n <= 40, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A082459 (Multiply by 1, add 1, multiply by 2, add 2, …) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) and a(1) are both -1 */
    if (n == 0 || n == 1)
        return (seq_t)-1;

    /* The largest step index we need to perform */
    idx_t max_k = (n + 1) / 2;

    seq_t val = (seq_t)-1;             /* value after step 0 (a(0)) */

    for (idx_t k = 1; k <= max_k; ++k) {
        seq_t prev = val * (seq_t)k;    /* a(2k-1) – value after multiplication, before addition */
        if (n == 2 * k - 1)
            return prev;

        val = prev + (seq_t)k;          /* a(2k) – value after addition */
        if (n == 2 * k)
            return val;
    }

    /* Should never reach here for n ≥ 2 */
    return val;
}
