/*
 * A207887  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n binary arrays avoiding * horizontal
 *            patterns 000 and 101, and vertical patterns 011 and 110. *
 *            Computed by dynamic programming over columns
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 39, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define MAXMASK (1 << 5)          /* 5‑bit column masks */
#define MAXSTATE (32 * 32)        /* 256 possible state pairs */

/* static data shared by all calls */
static bool          initialized = false;
static int           M;                          /* number of allowed masks   */
static int           mask_index[MAXMASK];         /* mask -> index (-1)        */
static uint32_t      allowed_mask[32];            /* allowed masks array       */
static uint32_t      trans[32][32];               /* transition bitmasks       */
static seq_t         dp_cur[MAXSTATE];            /* DP current array          */
static seq_t         dp_nxt[MAXSTATE];            /* DP next array             */

/* initialise tables once */
static void init_seq_A207887(void)
{
    if (initialized)
        return;

    /* initialise mapping */
    for (int i = 0; i < MAXMASK; ++i)
        mask_index[i] = -1;

    /* find allowed column masks (no vertical 011 or 110) */
    M = 0;
    for (int mask = 0; mask < MAXMASK; ++mask) {
        bool bad = false;
        for (int w = 0; w <= 5 - 3; ++w) {
            int trip = (mask >> w) & 0x7;       /* 3 consecutive bits */
            if (trip == 0x3 || trip == 0x6) {  /* 011 or 110 */
                bad = true;
                break;
            }
        }
        if (!bad) {
            mask_index[mask] = M;
            allowed_mask[M++] = (uint32_t)mask;
        }
    }

    /* precompute transition bitmasks */
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            uint32_t bits = 0;
            for (int k = 0; k < M; ++k) {
                bool ok = true;
                for (int r = 0; r < 5 && ok; ++r) {
                    int b0 = (allowed_mask[i] >> r) & 1;
                    int b1 = (allowed_mask[j] >> r) & 1;
                    int b2 = (allowed_mask[k] >> r) & 1;
                    if ((b0 == 0 && b1 == 0 && b2 == 0) ||
                        (b0 == 1 && b1 == 0 && b2 == 1))
                        ok = false;
                }
                if (ok)
                    bits |= (1U << k);
            }
            trans[i][j] = bits;
        }
    }

    initialized = true;
}

/* main user function */
seq_t a(idx_t n)
{
    init_seq_A207887();

    if (n == 0)
        return 0;

    if (n == 1)
        return (seq_t)M;

    if (n == 2) {
        seq_t v = (seq_t)M;
        return v * v;
    }

    /* initialize DP for the first two columns */
    for (int i = 0; i < M * M; ++i)
        dp_cur[i] = 1;

    /* iterate additional columns */
    for (idx_t step = 3; step <= n; ++step) {
        /* zero next array */
        memset(dp_nxt, 0, M * M * sizeof(seq_t));

        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < M; ++j) {
                seq_t cnt = dp_cur[i * M + j];
                if (!cnt)
                    continue;

                uint32_t bits = trans[i][j];
                for (int k = 0; k < M; ++k) {
                    if (bits & (1U << k))
                        dp_nxt[j * M + k] += cnt;
                }
            }
        }
        memcpy(dp_cur, dp_nxt, M * M * sizeof(seq_t));
    }

    /* sum all final states */
    seq_t result = 0;
    for (int idx = 0; idx < M * M; ++idx)
        result += dp_cur[idx];

    return result;
}
