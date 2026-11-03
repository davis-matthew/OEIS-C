/*
 * A228722  Largest number <= n with alternating parities of digits in decimal
 *          representation.
 * https://oeis.org/A228722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9690 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A228722: Largest number <= n with alternating parities of digits. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Retrieve the decimal digits of num in most‑significant‑first order.
   The buffer must have at least 20 elements (enough for 64‑bit numbers). */
static void get_digits_msb(idx_t num, uint8_t *buf, size_t *len)
{
    if (num == 0) {          /* special case for zero */
        buf[0] = 0;
        *len = 1;
        return;
    }

    uint8_t tmp[20];
    size_t cnt = 0;
    while (num > 0) {
        tmp[cnt++] = (uint8_t)(num % 10);
        num /= 10;
    }
    for (size_t i = 0; i < cnt; ++i) {
        buf[i] = tmp[cnt - 1 - i];
    }
    *len = cnt;
}

/* Recursive search for the largest alternating‑parity number of the same
   length as the array 'nd'.  nd[0] is the most significant digit. */
static bool dfs(size_t pos, bool tight, bool prevEven,
                seq_t cur, const uint8_t *nd, size_t len, seq_t *out)
{
    if (pos == len) {                /* full number constructed */
        *out = cur;
        return true;
    }

    uint8_t maxDigit = tight ? nd[pos] : 9;
    for (int d = maxDigit; d >= 0; --d) {
        if (pos == 0 && d == 0) continue;      /* no leading zero */

        bool even = (d % 2 == 0);
        if (pos > 0 && even == prevEven) continue;   /* parity conflict */

        bool newTight = tight && (d == nd[pos]);
        seq_t newCur = cur * (seq_t)10 + (seq_t)d;
        if (dfs(pos + 1, newTight, even, newCur, nd, len, out))
            return true;
    }
    return false;   /* no valid digit at this position */
}

/* Largest alternating‑parity number with 'len' digits. */
static seq_t build_pattern(size_t len)
{
    seq_t res = 0;
    for (size_t i = 0; i < len; ++i) {
        res = res * (seq_t)10 + ((i % 2 == 0) ? (seq_t)9 : (seq_t)8);
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* a(0) = 0 */

    /* Extract digits of n in msb‑first order. */
    uint8_t digits[20];
    size_t len;
    get_digits_msb(n, digits, &len);

    seq_t candidate;
    bool found = dfs(0, true, false, (seq_t)0, digits, len, &candidate);
    if (found) return candidate;

    /* No alternating number of this length <= n; return max of shorter length. */
    return build_pattern(len - 1);
}
