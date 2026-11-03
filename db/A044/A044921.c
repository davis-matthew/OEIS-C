/*
 * A044921  Numbers whose base-5 run lengths form a palindrome.
 * https://oeis.org/A044921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th smallest integer whose base‑5 run lengths form a
 *            palindrome
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool base5_runlen_palindrome(seq_t x)
{
    if (x == 0)                     /* base‑5 "0" has run‑length [1] */
        return true;

    int  digits[128];
    idx_t len = 0;
    seq_t tmp = x;

    /* extract base‑5 digits (least significant first) */
    while (tmp != 0) {
        digits[len++] = (int)(tmp % 5);
        tmp /= 5;
    }

    /* collect run lengths */
    int  rl[128];
    idx_t rl_cnt = 0;
    int  curr = digits[len - 1];
    idx_t run = 1;

    for (idx_t i = len - 1; i > 0; --i) {
        int d = digits[i - 1];
        if (d == curr)
            ++run;
        else {
            rl[rl_cnt++] = (int)run;
            curr = d;
            run = 1;
        }
    }
    rl[rl_cnt++] = (int)run;       /* last run */

    /* palindrome test */
    for (idx_t i = 0; i < rl_cnt / 2; ++i)
        if (rl[i] != rl[rl_cnt - 1 - i])
            return false;

    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)                     /* sequence is 1‑based; undefined for 0 */
        return 0;

    seq_t val   = 0;
    seq_t count = 0;

    while ((idx_t)count < n) {      /* cast to idx_t for reliable comparison */
        ++val;
        if (base5_runlen_palindrome(val))
            ++count;
    }
    return val;
}
