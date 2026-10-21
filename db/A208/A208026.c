/*
 * A208026  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 1 0 1 vertically.
 * https://oeis.org/A208026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A208026 – Number of n×6 binary arrays avoiding horizontally 000 and 001,
 *  and vertically 000 and 101.
 *  Computation is by dynamic programming over successive rows.
 *  Initial values: a(1)=26 , a(2)=676. */

#include <stdint.h>
#include <stdbool.h>

/* Maximum number of allowed 6-bit row patterns (≤ 26). */
#define MAXPAT 26

/* Static storage for pattern and transition data initialized once. */
static int      M = 0;                                /* number of patterns */
static int      pat[MAXPAT];                          /* pattern bitmasks   */
static uint32_t trans[MAXPAT][MAXPAT];                /* transition masks   */
static bool     patterns_initialized = false;        /* init flag          */

/* Build pattern list (those valid under horizontal constraints) and
 * transition table for vertical constraints. */
static void
init_patterns(void)
{
    int m = 0;
    for (int p = 0; p < 64; ++p) {
        bool ok = true;
        /* horizontal check: no 000 or 001 in the 6-bit row */
        for (int i = 0; i <= 3; ++i) {
            int b0 = (p >> i) & 1;
            int b1 = (p >> (i + 1)) & 1;
            int b2 = (p >> (i + 2)) & 1;
            if (b0 == 0 && b1 == 0 && (b2 == 0 || b2 == 1)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            pat[m++] = p;
        }
    }
    M = m;   /* expected to be 26 */

    /* build transition masks */
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            uint32_t mask = 0;
            int a = pat[i];
            int b = pat[j];
            for (int k = 0; k < M; ++k) {
                int c = pat[k];
                bool valid = true;
                for (int col = 0; col < 6; ++col) {
                    int bit_a = (a >> col) & 1;
                    int bit_b = (b >> col) & 1;
                    int bit_c = (c >> col) & 1;
                    /* vertical constraints */
                    if (bit_a == 0 && bit_b == 0 && bit_c == 0) {
                        valid = false;
                        break;
                    }
                    if (bit_a == 1 && bit_b == 0 && bit_c == 1) {
                        valid = false;
                        break;
                    }
                }
                if (valid) {
                    mask |= (1u << k);
                }
            }
            trans[i][j] = mask;
        }
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1 */

    /* Ensure patterns and transition data are ready. */
    if (!patterns_initialized) {
        init_patterns();
        patterns_initialized = true;
    }

    /* Handle trivial cases. */
    if (n == 1) return (seq_t)M;
    if (n == 2) return (seq_t)M * (seq_t)M;

    /* DP tables: states are pairs of consecutive rows. */
    static seq_t dp_cur[MAXPAT * MAXPAT];
    static seq_t dp_next[MAXPAT * MAXPAT];

    /* initialise dp_cur for the first two rows (all pairs allowed) */
    for (int i = 0; i < M; ++i) {
        int base = i * M;
        for (int j = 0; j < M; ++j) {
            dp_cur[base + j] = (seq_t)1;
        }
    }

    seq_t sum = 0;
    for (idx_t row = 3; row <= n; ++row) {
        /* reset dp_next */
        for (int idx = 0; idx < M * M; ++idx) {
            dp_next[idx] = (seq_t)0;
        }

        /* transition from state (i,j) to (j,k) */
        for (int i = 0; i < M; ++i) {
            int base = i * M;
            for (int j = 0; j < M; ++j) {
                seq_t val = dp_cur[base + j];
                if (!val) continue;
                uint32_t mask = trans[i][j];
                if (!mask) continue;
                for (int k = 0; k < M; ++k) {
                    if (mask & (1u << k)) {
                        dp_next[j * M + k] += val;
                    }
                }
            }
        }

        /* compute total for current row */
        sum = (seq_t)0;
        for (int idx = 0; idx < M * M; ++idx) {
            sum += dp_next[idx];
        }

        /* prepare for next iteration */
        for (int idx = 0; idx < M * M; ++idx) {
            dp_cur[idx] = dp_next[idx];
        }
    }

    return sum;
}
