/*
 * A238141  Number of (n+1) X (2+1) 0..3 arrays with the maximum plus the
 *          lower median minus the minimum of every 2 X 2 subblock differing
 *          from its horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A238141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays with entries in 0..3 such that *
 *            the value max+lower median−min of every 2×2 subblock differs
 *            from its * horizontal and vertical neighbours by exactly one.
 *            The implementation * uses dynamic programming over row patterns,
 *            keeping the value of the * left 2×2 block (f...
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   one_seq, sign_from_bit, zero_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/one_seq.h"
#include "../utils/sign_from_bit.h"
#include "../utils/zero_seq.h"

/* helper: evaluate max+lower median−min for four values */
static inline uint8_t block_value(uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3)
{
    uint8_t a[4] = { v0, v1, v2, v3 };
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3 - i; ++j)
            if (a[j] > a[j + 1]) {
                uint8_t t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
            }
    return a[3] + a[1] - a[0];               /* max + lower median – min */
}

struct PairInfo {
    bool   valid;
    uint8_t f;      /* f for column 0 block (0..6) */
    uint8_t sign;   /* 0 = –1, 1 = +1 */
};

/* 64 patterns for a row of 3 cells; each cell encoded by 2 bits */
static bool   initialized = false;
static uint8_t row_pat[64][3];
static struct PairInfo pair_tbl[64][64];

/* initialize pattern tables once */
static void init_tables(void)
{
    if (initialized) return;
    for (uint_fast8_t p = 0; p < 64; ++p) {
        row_pat[p][0] = (p >> 0) & 3;
        row_pat[p][1] = (p >> 2) & 3;
        row_pat[p][2] = (p >> 4) & 3;
    }
    for (uint_fast8_t p0 = 0; p0 < 64; ++p0)
        for (uint_fast8_t p1 = 0; p1 < 64; ++p1) {
            struct PairInfo *info = &pair_tbl[p0][p1];
            uint8_t a0 = row_pat[p0][0];
            uint8_t a1 = row_pat[p0][1];
            uint8_t a2 = row_pat[p0][2];
            uint8_t b0 = row_pat[p1][0];
            uint8_t b1 = row_pat[p1][1];
            uint8_t b2 = row_pat[p1][2];
            uint8_t f0 = block_value(a0, a1, b0, b1);
            uint8_t f1 = block_value(a1, a2, b1, b2);
            if ((f0 > f1 ? f0 - f1 : f1 - f0) != 1) {
                info->valid = false;
                continue;
            }
            info->valid = true;
            info->f     = f0;
            info->sign  = (f1 > f0) ? 1 : 0;   /* 1 => +1, 0 => –1 */
        }
    initialized = true;
}

seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return zero_seq();   /* not part of the sequence */

    /* DP arrays: [row pattern][f (0..6)][sign (0/1)]  */
    static seq_t dp_cur[64][7][2];
    static seq_t dp_next[64][7][2];

    /* clear dp_cur */
    for (uint_fast8_t p = 0; p < 64; ++p)
        for (uint_fast8_t f = 0; f < 7; ++f)
            for (uint_fast8_t s = 0; s < 2; ++s)
                dp_cur[p][f][s] = zero_seq();

    /* initial transition: first two rows */
    for (uint_fast8_t p0 = 0; p0 < 64; ++p0)
        for (uint_fast8_t p1 = 0; p1 < 64; ++p1) {
            const struct PairInfo *info = &pair_tbl[p0][p1];
            if (!info->valid) continue;
            dp_cur[p1][info->f][info->sign] += one_seq();
        }

    /* subsequent transitions */
    for (idx_t step = 1; step < n; ++step) {
        for (uint_fast8_t p = 0; p < 64; ++p)
            for (uint_fast8_t f = 0; f < 7; ++f)
                for (uint_fast8_t s = 0; s < 2; ++s)
                    dp_next[p][f][s] = zero_seq();

        for (uint_fast8_t p_prev = 0; p_prev < 64; ++p_prev) {
            for (uint_fast8_t f_prev = 0; f_prev < 7; ++f_prev) {
                for (uint_fast8_t s_prev = 0; s_prev < 2; ++s_prev) {
                    seq_t ways = dp_cur[p_prev][f_prev][s_prev];
                    if (ways == zero_seq()) continue;
                    int  sig_prev = sign_from_bit(s_prev);
                    int  f_prev1  = f_prev + sig_prev;

                    for (uint_fast8_t p_cur = 0; p_cur < 64; ++p_cur) {
                        const struct PairInfo *info = &pair_tbl[p_prev][p_cur];
                        if (!info->valid) continue;
                        int  sig_cur = sign_from_bit(info->sign);
                        int  f_cur  = info->f;
                        int  f_cur1 = f_cur + sig_cur;

                        if ((f_cur > f_prev ? f_cur - f_prev : f_prev - f_cur) != 1) continue;
                        if ((f_cur1 > f_prev1 ? f_cur1 - f_prev1 : f_prev1 - f_cur1) != 1) continue;

                        dp_next[p_cur][f_cur][info->sign] += ways;
                    }
                }
            }
        }

        /* swap dp_cur and dp_next */
        for (uint_fast8_t p = 0; p < 64; ++p)
            for (uint_fast8_t f = 0; f < 7; ++f)
                for (uint_fast8_t s = 0; s < 2; ++s)
                    dp_cur[p][f][s] = dp_next[p][f][s];
    }

    /* sum all end states */
    seq_t result = zero_seq();
    for (uint_fast8_t p = 0; p < 64; ++p)
        for (uint_fast8_t f = 0; f < 7; ++f)
            for (uint_fast8_t s = 0; s < 2; ++s)
                result += dp_cur[p][f][s];

    return result;
}
