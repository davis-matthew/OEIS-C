/*
 * A045927  Digits even, nonzero and nondecreasing.
 * https://oeis.org/A045927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   comb_repeat
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/comb_repeat.h"
/* Non‑prime even non‑zero non‑decreasing digit numbers.
   Sorted by increasing numeric value.
   The nth term is obtained by counting weakly increasing sequences
   of length L over the alphabet {2,4,6,8}.  */

/* Digits allowed (sorted). */
static unsigned char DIGITS[4] = {2,4,6,8};

seq_t a(idx_t n)
{
    if (n == 0U) return 0;               /* behaviour below the offset is irrelevant */

    /* Find the length L of the nth number. */
    idx_t cum = 0U;
    int len = 1;
    while (1) {
        idx_t cnt = comb_repeat(len, 4U);   /* C(len+3,3) numbers of length len */
        if (cum + cnt >= n) break;
        cum += cnt;
        ++len;
    }

    /* Zero‑based offset within all numbers of the found length. */
    idx_t offset = n - cum - 1U;
    seq_t val = 0;
    int minidx = 0;                     /* smallest index of digit allowed at current position */

    /* Build the digits from most significant to least significant. */
    for (int pos = 0; pos < len; ++pos) {
        for (int di = minidx; di < 4; ++di) {
            idx_t cnt = comb_repeat(len - pos - 1, 4 - di);
            if (offset >= cnt) {
                offset -= cnt;
                continue;
            }
            val = val * 10 + DIGITS[di];
            minidx = di;                /* digits cannot decrease afterwards */
            break;
        }
    }
    return val;
}
