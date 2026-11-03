/*
 * A081607  Number of numbers <= n having at least one 0 in their ternary
 *          representation.
 * https://oeis.org/A081607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 316, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9153 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = (n+1) – C(n)  where C(n) counts the numbers ≤ n whose ternary
 *  representation contains no zero digit. The function uses a digit‑DP
 *  that works for any integer type seq_t and idx_t. */
#include <stddef.h>
#include <stdbool.h>

static seq_t count_no_zero(idx_t n)
{
    if (n == 0) return 0;                 /* 0 itself contains a zero digit */

    /* Convert n to base‑3 digits (most significant first). */
    int digits[70];
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0 && len < sizeof(digits)/sizeof(digits[0])) {
        digits[len++] = (int)(tmp % 3);
        tmp /= 3;
    }
    /* reverse to most significant first */
    for (size_t i = 0; i < len/2; ++i) {
        int t = digits[i];
        digits[i] = digits[len-1-i];
        digits[len-1-i] = t;
    }

    /* DP: cur[started][tight] */
    seq_t cur[2][2] = { {0} };
    cur[0][1] = 1;                         /* before any digits, no start, tight */

    for (size_t pos = 0; pos < len; ++pos) {
        seq_t nxt[2][2] = { {0} };
        for (int started = 0; started <= 1; ++started) {
            for (int tight = 0; tight <= 1; ++tight) {
                seq_t val = cur[started][tight];
                if (!val) continue;
                int limit = tight ? digits[pos] : 2;
                for (int d = 0; d <= limit; ++d) {
                    if (started && d == 0) continue;          /* zero after start forbidden */
                    int nstarted = started | (d != 0);
                    int ntight   = tight && (d == digits[pos]);
                    nxt[nstarted][ntight] += val;
                }
            }
        }
        /* move to next position */
        cur[0][0] = nxt[0][0];
        cur[0][1] = nxt[0][1];
        cur[1][0] = nxt[1][0];
        cur[1][1] = nxt[1][1];
    }

    /* Numbers counted with at least one non‑zero digit (i.e., >0) */
    return cur[1][0] + cur[1][1];
}

seq_t a(idx_t n)
{
    seq_t noZero = count_no_zero(n);
    seq_t total  = (seq_t)n + (seq_t)1;
    return total - noZero;
}
