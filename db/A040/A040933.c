/*
 * A040933  Continued fraction for sqrt(965).
 * https://oeis.org/A040933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040933: continued fraction for sqrt(965) */
/* a(0) = 31, thereafter the period‑5 block 15,1,1,15,62 repeats */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)31;
    static const seq_t block[5] = {
        (seq_t)15,
        (seq_t)1,
        (seq_t)1,
        (seq_t)15,
        (seq_t)62
    };
    /* n >= 1 here */
    size_t idx = (size_t)((n - 1) % 5);
    return block[idx];
}
