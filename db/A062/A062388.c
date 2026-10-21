/*
 * A062388  Smallest palindrome with digit sum = n.
 * https://oeis.org/A062388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 81, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = smallest decimal palindrome whose digit sum equals n
   (OEIS sequence A062388).  The function constructs the palindrome by
   checking lengths in increasing order and building the lexicographically
   minimal digits that satisfy the required sum.  No hard‑coded table is used. */

#include <stdint.h>
#include <stdbool.h>

#define MAX_LEFT 2000  /* sufficient for n up to 10000 */

static seq_t build_even(idx_t L, idx_t n)
{
    idx_t k = L / 2;
    if ((n & 1) != 0) return 0;                 /* sum must be even */
    idx_t s_d = n / 2;
    if (s_d > 9 * k) return 0;                  /* impossible sum */

    uint8_t left[MAX_LEFT];
    idx_t remaining = s_d;
    for (idx_t i = 0; i < k; ++i) {
        idx_t remPos = k - i - 1;
        idx_t lower = (i == 0) ? 1 : 0;
        idx_t max_rem = 9 * remPos;
        idx_t digit = remaining > max_rem ? remaining - max_rem : lower;
        if (digit > 9) return 0;                /* should not happen */
        left[i] = (uint8_t)digit;
        remaining -= digit;
    }
    if (remaining != 0) return 0;               /* safety check */

    seq_t res = 0;
    for (idx_t i = 0; i < k; ++i) res = res * 10 + left[i];
    for (idx_t i = k; i > 0; --i) res = res * 10 + left[i - 1];
    return res;
}

static seq_t build_odd(idx_t L, idx_t n)
{
    idx_t k = L / 2;        /* floor((L-1)/2) */
    if (k == 0) {           /* L == 1 */
        if (n <= 9) return (seq_t)n;
        return 0;
    }

    seq_t best = 0;
    bool found = false;

    for (uint8_t m = 0; m <= 9; ++m) {            /* middle digit */
        if ((n >= m) && ((n - m) & 1) == 0) {
            idx_t s_d = (n - m) / 2;
            if (s_d > 9 * k) continue;

            uint8_t left[MAX_LEFT];
            idx_t remaining = s_d;
            bool ok = true;
            for (idx_t i = 0; i < k; ++i) {
                idx_t remPos = k - i - 1;
                idx_t lower = (i == 0) ? 1 : 0;
                idx_t max_rem = 9 * remPos;
                idx_t digit = remaining > max_rem ? remaining - max_rem : lower;
                if (digit > 9) { ok = false; break; }
                left[i] = (uint8_t)digit;
                remaining -= digit;
            }
            if (!ok || remaining != 0) continue;

            seq_t val = 0;
            for (idx_t i = 0; i < k; ++i) val = val * 10 + left[i];
            val = val * 10 + m;
            for (idx_t i = k; i > 0; --i) val = val * 10 + left[i - 1];

            if (!found || val < best) {
                best = val;
                found = true;
            }
        }
    }
    return found ? best : 0;
}

seq_t a(idx_t n)
{
    if (n <= 9) return (seq_t)n;       /* single‑digit palindromes */

    for (idx_t L = 2;; ++L) {
        seq_t val = (L & 1) ? build_odd(L, n) : build_even(L, n);
        if (val) return val;
    }
}
