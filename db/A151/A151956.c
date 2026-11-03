/*
 * A151956  a(1) = 1002; thereafter a(n) = (a(n-1) with digits sorted into
 *          descending order) - (a(n-1) with digits sorted into ascending
 *          order) (see the Kaprekar map, A151949).
 * https://oeis.org/A151956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1002; a(n) = (digits of a(n‑1) sorted descending) –
 *            (digits of a(n‑1) sorted ascending) (Kaprekar map for 4‑digit
 *            numbers)
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void digits_sort_decasc(seq_t value, seq_t *asc, seq_t *desc)
{
    /* Extract up to 4 digits (with leading zeros). */
    uint8_t dig[4];
    int  len = 0;
    seq_t tmp = value;
    while (tmp > 0 && len < 4) {
        dig[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    /* Pad with zeros if necessary. */
    while (len < 4) {
        dig[len++] = 0;
    }

    /* Simple insertion sort for the very small array. */
    for (int i = 1; i < len; ++i) {
        uint8_t x = dig[i];
        int j = i - 1;
        while (j >= 0 && dig[j] > x) {
            dig[j + 1] = dig[j];
            --j;
        }
        dig[j + 1] = x;
    }

    /* Build ascending number. */
    *asc = 0;
    for (int i = 0; i < len; ++i) {
        *asc = *asc * 10 + dig[i];
    }
    /* Build descending number (reverse). */
    *desc = 0;
    for (int i = len - 1; i >= 0; --i) {
        *desc = *desc * 10 + dig[i];
    }
}

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;          /* Undefined for n < 1, but return 0 for safety. */
    }

    seq_t cur = 1002;      /* a(1) */

    /* For n == 1, return immediately. */
    if (n == 1) {
        return cur;
    }

    for (idx_t i = 2; i <= n; ++i) {
        seq_t asc, desc;
        digits_sort_decasc(cur, &asc, &desc);
        cur = desc - asc;

        /* After reaching the fixed point 6174, subsequent terms are the same. */
        if (cur == 6174) {
            return 6174;
        }
    }
    return cur;
}
