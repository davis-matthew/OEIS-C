/*
 * A069697  Triangular numbers with internal digits 5.
 * https://oeis.org/A069697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangular numbers with internal digits 5
   (a(n) = n(n+1)/2 if the number has middle digit(s) equal to 5;
   all 1‑ or 2‑digit triangular numbers are included) */
#include <stdbool.h>
#include "../utils/pow10.h"

static bool has_middle_five(seq_t x)
{
    /* 1‑ or 2‑digit numbers are always accepted */
    if (x <= 99) return true;

    /* Count decimal digits */
    idx_t len = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        tmp /= 10;
        len++;
    }

    if (len <= 2) return true;  /* safety, though x > 99 guarantees len > 2 */

    if (len % 2 == 1) {         /* odd length – single middle digit */
        idx_t exp = (len - 1) / 2;
        seq_t div = pow10(exp);
        seq_t mid = (x / div) % 10;
        return mid == 5;
    } else {                    /* even length – two middle digits */
        idx_t exp_low = len / 2;
        seq_t div_low = pow10(exp_low);
        seq_t mid_low = (x / div_low) % 10;
        seq_t div_high = pow10(exp_low - 1);
        seq_t mid_high = (x / div_high) % 10;
        return (mid_low == 5 && mid_high == 5);
    }
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 0; ; k++) {
        seq_t t = (seq_t)k * ((seq_t)k + 1) / 2;   /* T(k) */
        if (has_middle_five(t)) {
            count++;
            if (count == n) return t;
        }
    }
}
