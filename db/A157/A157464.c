/*
 * A157464  Number of distinct squares when inserting at most one digit in
 *          decimal representation of n.
 * https://oeis.org/A157464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = # of distinct squares obtainable by inserting at most one
 *            decimal digit into the decimal representation of n
 * Range:     u8 n <= 9, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9083 (2026-11-01)
 * Helpers:   ipow_i32, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"
#include "../utils/num_digits_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static inline seq_t seq_one(void) { return (seq_t)1; }

seq_t a(idx_t n)
{
    /* Length of n in decimal digits */
    int m = num_digits_idx(n);           /* 0 -> 1 */
    int target_len = m + 1;          /* length of the padded square string */

    /* Compute upper bound: 10^target_len - 1 */
    seq_t pow10_val = pow10((idx_t)target_len);
    seq_t one = seq_one();
    seq_t bound = pow10_val - one;

    /* Convert n to string */
    char n_buf[128];
    size_t n_len;
    seq_to_str((seq_t)n, n_buf, &n_len);
    n_buf[n_len] = '\0';

    seq_t count = 0;
    idx_t i = 0;
    for (;;) {
        seq_t sq = seq_mul((seq_t)i, (seq_t)i);   /* i^2 */
        if (sq > bound) break;                    /* larger squares unnecessary */

        /* Convert square to string */
        char s_buf[128];
        size_t s_len;
        seq_to_str(sq, s_buf, &s_len);
        s_buf[s_len] = '\0';

        /* Pad with leading zeros to target_len */
        int pad = target_len - (int)s_len;
        if (pad < 0) pad = 0;                     /* safety, should not happen */

        char padded[128];
        size_t padded_len = pad + s_len;
        for (int k = 0; k < pad; ++k) padded[k] = '0';
        memcpy(padded + pad, s_buf, s_len);
        padded[padded_len] = '\0';

        /* Check if deleting exactly one character from padded gives n */
        bool matched = false;
        for (int del = 0; del < padded_len && !matched; ++del) {
            size_t p = 0, q = 0;
            while (p < padded_len && q < n_len) {
                if ((int)p == del) { ++p; continue; }  /* skip the deleted char */
                if (padded[p] != n_buf[q]) break;
                ++p; ++q;
            }
            if (q == n_len) matched = true;
        }

        if (matched) count++;

        ++i;
    }

    return count;
}
