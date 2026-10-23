/*
 * A122468  Multiply the first 2 digits and then add and multiply alternately
 *          the next digits.
 * https://oeis.org/A122468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=763592; for n>1, a(n)=f(a(n-1)) where f performs multiply
 *            first two decimal digits, then alternately add and multiply the
 *            next digits
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */

static seq_t transform(seq_t x) {
    /* A single-digit number is unchanged. */
    if (x < (seq_t)10) return x;

    /* Extract decimal digits into an array (least‑significant first). */
    seq_t tmp = x;
    seq_t digits[256];
    size_t len = 0;
    while (tmp > (seq_t)0) {
        digits[len++] = tmp % (seq_t)10;
        tmp /= (seq_t)10;
    }

    /* Multiply the first two digits (most‑significant). */
    seq_t v = digits[len-1] * digits[len-2];

    /* Alternately add and multiply the remaining digits, starting with add. */
    int add = 1;   /* 1 → addition, 0 → multiplication */
    for (size_t i = len-3; i < len; --i) {
        if (add) {
            v += digits[i];
            add = 0;
        } else {
            v *= digits[i];
            add = 1;
        }
    }
    return v;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* safety guard; offset is 1 */
    seq_t val = (seq_t)763592;              /* a(1) */
    for (idx_t i = 1; i < n; ++i)          /* iterate n‑1 times */
        val = transform(val);
    return val;
}
