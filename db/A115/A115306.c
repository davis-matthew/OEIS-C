/*
 * A115306  a(n) = if n<6 then n else 6*a(floor(n/6)) + 5 - n mod 6.
 * https://oeis.org/A115306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n for n<6; for n≥6, a(n) = 6*a(floor(n/6)) + 5 - (n mod
 *            6)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A115306 */
#include <stddef.h>

seq_t a(idx_t n) {
    if (n < 6) {
        return (seq_t)n;
    }

    /* Extract base‑6 digits (least significant first) */
    idx_t tmp = n;
    uint8_t digits[64];
    size_t len = 0;
    while (tmp) {
        digits[len++] = (uint8_t)(tmp % 6);
        tmp /= 6;
    }

    /* Reconstruct the number with digits transformed:
       leading digit stays the same, others become 5 - d */
    seq_t result = 0;
    seq_t mult = 1;
    for (size_t i = 0; i < len; ++i) {
        seq_t d = (seq_t)digits[i];
        seq_t nd;
        if (i == len - 1) {
            nd = d;                 /* leading digit unchanged */
        } else {
            nd = (seq_t)(5 - d);     /* replace with 5-d */
        }
        result += nd * mult;
        mult *= 6;
    }

    return result;
}
