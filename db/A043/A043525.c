/*
 * A043525  Numbers having one 9 in base 10.
 * https://oeis.org/A043525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer having exactly one decimal digit 9. */

#include <stdbool.h>

/* 9^e as seq_t.  e is small enough for any requested n. */
static seq_t pow9(idx_t e)
{
    seq_t r = 1;
    while (e--) r *= 9;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0, but harmless */

    /* Find the decimal length L of a(n). */
    idx_t L = 1;
    seq_t cumulative = 0;
    while (1) {
        seq_t cnt;
        seq_t pow9_Lm1 = pow9(L - 1);              /* 9^(L-1) */
        cnt = pow9_Lm1;                           /* numbers starting with 9 */

        if (L >= 2) {
            seq_t pow9_Lm2 = pow9(L - 2);          /* 9^(L-2) */
            cnt += (seq_t)8 * (L - 1) * pow9_Lm2; /* leading 1–8, 9 later */
        }

        if (cumulative + cnt >= (seq_t)n) break;
        cumulative += cnt;
        ++L;
    }

    seq_t offset = (seq_t)n - cumulative - 1; /* 0‑based offset within L‑digit block */

    /* Build the digits one by one from most significant to least. */
    uint8_t digits[64] = {0};
    bool has9 = false;
    idx_t pos = 0;

    while (pos < L) {
        idx_t remaining = L - pos - 1;
        idx_t d_start = (pos == 0) ? 1 : 0;

        for (idx_t d = d_start; d <= 9; ++d) {
            if (has9 && d == 9) continue;  /* cannot place a second 9 */

            bool new_has9 = has9 || (d == 9);
            seq_t count;

            if (remaining == 0) {
                count = new_has9 ? 1 : 0;
            } else {
                if (new_has9) {
                    count = pow9(remaining);               /* all remaining digits 0‑8 */
                } else {
                    count = (seq_t)remaining * pow9(remaining - 1);
                }
            }

            if (offset < count) {
                digits[pos] = (uint8_t)d;
                has9 = new_has9;
                break;
            } else {
                offset -= count;
            }
        }
        ++pos;
    }

    /* Convert the digit array to a number. */
    seq_t result = 0;
    for (idx_t i = 0; i < L; ++i)
        result = result * 10 + digits[i];

    return result;
}
