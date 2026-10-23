/*
 * A043397  Numbers whose base-7 representation contains exactly one 1.
 * https://oeis.org/A043397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043397: Numbers whose base-7 representation contains exactly one 1. */
/* Implementation counts numbers by digit length and builds the nth number directly. */
#include <stdint.h>

static idx_t pow6[65];

static void init_pow6(void) {
    static int done = 0;
    if (done) return;
    pow6[0] = 1;
    for (int i = 1; i < 65; ++i) pow6[i] = pow6[i - 1] * 6ULL;
    done = 1;
}

seq_t a(idx_t n) {
    /* initialise powers of 6 once */
    init_pow6();

    /* find the length L of the n-th number */
    idx_t cum = 0;
    idx_t L = 0;
    for (;;) {
        ++L;
        idx_t f;
        if (L == 1) {
            f = 1;
        } else {
            f = pow6[L - 1] + (idx_t)(L - 1) * 5ULL * pow6[L - 2];
        }
        if (cum + f >= n) break;
        cum += f;
    }

    idx_t rank_within = n - cum - 1;           /* 0‑based inside length L block */
    seq_t msd;                                 /* most significant digit */
    idx_t rest_len;
    idx_t rest_rank;

    if (rank_within < pow6[L - 1]) {
        /* the one is in the most significant position */
        msd = 1;
        rest_rank = rank_within;
        rest_len = L - 1;
    } else {
        /* the one is in one of the remaining positions */
        rank_within -= pow6[L - 1];
        idx_t block = (idx_t)(L - 1) * pow6[L - 2];
        idx_t msd_idx = rank_within / block;
        msd = 2 + msd_idx;
        rank_within %= block;
        rest_rank = rank_within;
        rest_len = L - 1;
    }

    /* build the rest of the digits (least significant at rest digits array end) */
    unsigned char rest[64];   /* maximum possible length is small (<64) */
    idx_t r = rest_rank;
    int hasPlaced1 = (msd == 1) ? 1 : 0;

    for (idx_t pos = 0; pos < rest_len; ++pos) {
        idx_t rem = rest_len - pos;   /* how many positions left including current */
        int chosen = -1;
        for (int d = 0; d <= 6; ++d) {
            if (hasPlaced1 && d == 1) continue;   /* digit 1 not allowed after msd=1 */

            idx_t cnt;
            if (hasPlaced1) {
                cnt = pow6[rem - 1];
            } else {
                if (d == 1) {
                    cnt = pow6[rem - 1];
                } else {
                    if (rem == 1) {
                        cnt = 0;
                    } else {
                        cnt = (rem - 1) * pow6[rem - 2];
                    }
                }
            }
            if (cnt == 0) continue;

            if (r >= cnt) {
                r -= cnt;
            } else {
                chosen = d;
                if (!hasPlaced1 && d == 1) hasPlaced1 = 1;
                break;
            }
        }
        if (chosen == -1) chosen = 6;   /* fall back, should not happen */
        rest[pos] = (unsigned char)chosen;
    }

    /* convert digits to the resulting integer */
    seq_t result = msd;
    for (idx_t pos = 0; pos < rest_len; ++pos) {
        result = result * 7 + rest[pos];
    }
    return result;
}
