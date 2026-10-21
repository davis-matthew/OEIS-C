/*
 * A205993  Number of (n+1)X4 0..2 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock equal to two, and every 2X2
 *          determinant nonzero.
 * https://oeis.org/A205993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A205993 – Number of (n+1)×4 0..2 arrays with
 * exactly two clockwise edge increases in each 2×2 block
 * and non‑zero determinant for every 2×2 block.
 *
 * The sequence satisfies the 19‑term linear recurrence
 *
 *   a(n) =  6 a(n−1)  + 40 a(n−2)  −160 a(n−3)  −206 a(n−4)
 *       +1227 a(n−5) + 360 a(n−6)  −5565 a(n−7)  +4196 a(n−8)
 *       +4799 a(n−9)  −3146 a(n−10) −3968 a(n−11) −1311 a(n−12)
 *       − 636 a(n−13) − 64 a(n−14) +210 a(n−15) +588 a(n−16)
 *       +228 a(n−17)  + 32 a(n−18)  − 16 a(n−19)      (n > 20)
 *
 * Initial values a(1) … a(20) are computed by a transfer‑matrix
 * dynamic program that enumerates all 81 possible 4‑cell rows.
 *
 * The implementation is intentionally simple: it precomputes
 * the 20 initial terms once, then continues the recurrence when
 * requested terms are larger.  All calculations use the arbitrary
 * precision type `seq_t` provided by the build system.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

#define STATES 81

/* Transition matrix: 1 if two rows can follow each other. */
static unsigned char TRANS[STATES][STATES];

/* Decoded digits of each row state. */
static unsigned char DIGIT[STATES][4];

/* Sequence cache (1‑based). CACHE[0] is unused. */
static seq_t *CACHE = NULL;
static size_t CACHE_LEN = 0;   /* number of stored elements (highest index + 1) */
static size_t CACHE_CAP = 0;   /* allocated capacity */

/* ------------------------------------------------------------------ */
/*   Precompute transition matrix and the first 20 terms                */
/* ------------------------------------------------------------------ */
static void init_cache(void)
{
    if (CACHE_LEN > 0)   /* already initialized */
        return;

    /* Decode each state into its four digits (base‑3). */
    for (int s = 0; s < STATES; ++s) {
        int v = s;
        for (int k = 3; k >= 0; --k) {
            DIGIT[s][k] = v % 3;
            v /= 3;
        }
    }

    /* Build TRANS. */
    for (int i = 0; i < STATES; ++i) {
        for (int j = 0; j < STATES; ++j) {
            int ok = 1;
            /* Check the three 2×2 blocks horizontally. */
            for (int c = 0; c < 3; ++c) {
                int a = DIGIT[i][c];
                int b = DIGIT[i][c + 1];
                int c1 = DIGIT[j][c];
                int d = DIGIT[j][c + 1];

                /* Count clockwise edge increases. */
                int inc = (a < b) + (b < d) + (d < c1) + (c1 < a);
                if (inc != 2) {
                    ok = 0;
                    break;
                }

                /* Non‑zero determinant: a*d ≠ b*c1. */
                if (a * d == b * c1) {
                    ok = 0;
                    break;
                }
            }
            TRANS[i][j] = (unsigned char)ok;
        }
    }

    /* Allocate cache for at least 21 entries (1‑based). */
    CACHE_CAP = 32;
    CACHE = seq_alloc_new_array(CACHE_CAP);

    /* Temporary DP vectors for the transfer matrix. */
    seq_t *dp_cur = seq_alloc_new_array(STATES);
    seq_t *dp_next = seq_alloc_new_array(STATES);

    /* First row can be any state. */
    for (int s = 0; s < STATES; ++s)
        dp_cur[s] = 1;

    /* Compute a(1)…a(20). */
    for (int step = 1; step <= 20; ++step) {
        /* Zero dp_next. */
        for (int s = 0; s < STATES; ++s)
            dp_next[s] = 0;

        /* Transition. */
        for (int i = 0; i < STATES; ++i) {
            seq_t val = dp_cur[i];
            if (val == 0)
                continue;
            for (int j = 0; j < STATES; ++j) {
                if (TRANS[i][j])
                    dp_next[j] = dp_next[j] + val;
            }
        }

        /* Sum all entries for a(step). */
        seq_t sum = 0;
        for (int s = 0; s < STATES; ++s)
            sum = sum + dp_next[s];

        CACHE[step] = sum;   /* store term */

        /* Prepare next iteration. */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* We now have terms 1…20 cached. */
    CACHE_LEN = 21;   /* highest index stored + 1 */

    /* Free temporary DP arrays. */
    seq_free_array(dp_cur);
    seq_free_array(dp_next);
}

/* ------------------------------------------------------------------ */
/*   Extend CACHE up to index n (inclusive) using the recurrence.    */
/* ------------------------------------------------------------------ */
static void ensure_cache_up_to(idx_t n)
{
    if (n < CACHE_LEN)
        return;   /* already available */

    /* Grow CACHE to accommodate index n. */
    while (CACHE_CAP <= (size_t)n) {
        size_t new_cap = CACHE_CAP * 2;
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < CACHE_CAP; ++i)
            new_cache[i] = CACHE[i];
        seq_free_array(CACHE);
        CACHE = new_cache;
        CACHE_CAP = new_cap;
    }

    /* Recurrence coefficients (19 terms). */
    static const int COEFF[19] = {
        6,   40,  -160, -206, 1227, 360,
       -5565, 4196, 4799, -3146, -3968, -1311,
       -636, -64, 210, 588, 228, 32, -16
    };

    /* Compute terms from CACHE_LEN to n. */
    for (idx_t m = CACHE_LEN; m <= n; ++m) {
        seq_t val = 0;
        for (int k = 0; k < 19; ++k) {
            idx_t idx = m - (k + 1);      /* a(m-(k+1)) */
            seq_t term = CACHE[idx];
            int c = COEFF[k];
            if (c >= 0)
                val = val + (seq_t)c * term;
            else
                val = val - (seq_t)(-c) * term;
        }
        CACHE[m] = val;
    }

    CACHE_LEN = (size_t)n + 1;
}

/* ------------------------------------------------------------------ */
/*   Public function required by the framework.                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)          /* the offset is 1; values below are undefined */
        return 0;

    init_cache();
    ensure_cache_up_to(n);
    return CACHE[(size_t)n];
}
