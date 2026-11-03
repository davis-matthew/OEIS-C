/*
 * A044912  Numbers whose base-11 run lengths alternate: odd, even, odd, ...
 * https://oeis.org/A044912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static bool _a044912_is_valid(seq_t x)
{
    if (x == 0) return false;               /* not used for n ≥ 1 */

    /* collect base‑11 digits (least significant first) */
    uint8_t digits[256];
    size_t len = 0;
    seq_t t = x;
    while (t > 0) {
        digits[len++] = (uint8_t)(t % (seq_t)11);
        t /= (seq_t)11;
    }

    /* process runs from most significant digit to least */
    int prev_parity = -1;                   /* -1 = no run yet */
    size_t i = len;                         /* start just past most significant */
    while (i > 0) {
        uint8_t cur = digits[--i];          /* most significant digit now at i */

        size_t run = 1;
        while (i > 0 && digits[i - 1] == cur) {
            run++;
            i--;
        }

        int parity = (int)(run & 1);        /* 1 = odd, 0 = even */
        if (prev_parity == -1) {
            if (parity != 1) return false; /* first run must be odd */
        } else {
            if (parity == prev_parity) return false; /* parity must alternate */
        }
        prev_parity = parity;
    }
    return true;
}

/* OEIS A044912: numbers whose base‑11 run lengths alternate odd, even, odd, ...
   Algorithm: check each integer in order; a number is valid if the runs of
   equal digits in its base‑11 expansion start with an odd length and then
   alternate parity.                                                        */

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t val = 1;
    while (count < n) {
        if (_a044912_is_valid(val)) {
            count++;
            if (count == n) return val;
        }
        val++;
    }
    return 0;   /* unreachable */
}
