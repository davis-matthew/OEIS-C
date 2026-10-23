/*
 * A183842  Number of (n+1)X7 binary arrays with each element of every 2X2
 *          subblock being the sum mod 2 of two others.
 * https://oeis.org/A183842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A183842: Number of (n+1)×7 binary arrays with each 2×2 subblock
 * satisfying the "sum modulo 2 of two others" condition.
 * Computed by dynamic programming over rows using a 128×128
 * transition matrix derived from the 13 admissible 4‑bit patterns
 * for a 2×2 block.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define STATE (1 << 7)   /* 7‑bit rows */

/* 13 admissible 4‑bit patterns (weight 0, 2, or 3) */
static const bool VALID_4BIT[16] = {
    true,  /* 0000 */
    false, /* 0001 */
    false, /* 0010 */
    true,  /* 0011 */
    false, /* 0100 */
    true,  /* 0101 */
    true,  /* 0110 */
    true,  /* 0111 */
    false, /* 1000 */
    true,  /* 1001 */
    true,  /* 1010 */
    true,  /* 1011 */
    true,  /* 1100 */
    true,  /* 1101 */
    true,  /* 1110 */
    false  /* 1111 */
};

static uint8_t TRANS[STATE][STATE];
static bool TRANS_READY = false;

static void build_transition(void)
{
    if (TRANS_READY)
        return;

    for (size_t a = 0; a < STATE; ++a) {
        for (size_t b = 0; b < STATE; ++b) {
            bool ok = true;
            /* check the 6 overlapping 2×2 blocks */
            for (size_t k = 0; k < 6; ++k) {
                unsigned pat = ((a >> k) & 1) << 3 |
                               ((a >> (k + 1)) & 1) << 2 |
                               ((b >> k) & 1) << 1 |
                               ((b >> (k + 1)) & 1);
                if (!VALID_4BIT[pat]) {
                    ok = false;
                    break;
                }
            }
            TRANS[a][b] = ok ? 1 : 0;
        }
    }
    TRANS_READY = true;
}

/* Function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for offset 1, but harmless */

    build_transition();

    static seq_t dp[STATE];
    static seq_t dp_next[STATE];

    /* initialise with one arbitrary first row */
    for (size_t i = 0; i < STATE; ++i)
        dp[i] = 1;

    /* perform n transitions to obtain (n+1) rows */
    for (idx_t step = 0; step < n; ++step) {
        for (size_t i = 0; i < STATE; ++i)
            dp_next[i] = 0;

        for (size_t a = 0; a < STATE; ++a) {
            seq_t val = dp[a];
            if (val == 0)
                continue;
            for (size_t b = 0; b < STATE; ++b) {
                if (TRANS[a][b])
                    dp_next[b] += val;
            }
        }

        for (size_t i = 0; i < STATE; ++i)
            dp[i] = dp_next[i];
    }

    /* sum all possible final rows */
    seq_t total = 0;
    for (size_t i = 0; i < STATE; ++i)
        total += dp[i];

    return total;
}
