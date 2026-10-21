/*
 * A207777  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 1 1 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of 7×n binary arrays avoiding the patterns horizontally
 *            000,111 and vertically 001,010. Computed by dynamic programming
 *            on column patterns
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
#ifdef __cplusplus
#include <vector>
#include <cstddef>
#include <cstring>
#endif
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/*  Constants and static data structures                         */
/* ------------------------------------------------------------ */
#define MAXS 64                     /* ≥ possible number of vertical‑valid columns */

/* Arrays of column patterns */
static uint8_t valid_cols[MAXS];   /* column bit patterns (7 bits) */
static int col_index[128];         /* index of a 7‑bit pattern or -1 if invalid */

static int S;                      /* number of valid columns (≤ MAXS) */
static int S2;                     /* S*S */

/* Transition tables: from pair (a,b) to allowed next columns c */
static int adj_len[MAXS*MAXS];                      /* number of neighbours for pair */
static uint8_t adj_list[MAXS*MAXS*MAXS];            /* packed neighbour indices */

/* DP vectors for two consecutive columns */
#ifdef __cplusplus
static std::vector<seq_t> dp_cur, dp_next;
#else
static seq_t *dp_cur, *dp_next;
#endif

/* Build flag */
static bool built = false;

/* ------------------------------------------------------------ */
/*  Helper: test a single column for vertical constraints     */
/* ------------------------------------------------------------ */
static bool valid_column(uint8_t col)
{
    for (int i = 0; i < 5; ++i) {
        int b1 = (col >> i)     & 1;
        int b2 = (col >> (i+1)) & 1;
        int b3 = (col >> (i+2)) & 1;
        if (b1 == 0 && b2 == 0 && b3 == 1) return false; /* 001 */
        if (b1 == 0 && b2 == 1 && b3 == 0) return false; /* 010 */
    }
    return true;
}

/* ------------------------------------------------------------ */
/*  Initialize all tables once                                  */
/* ------------------------------------------------------------ */
static void build(void)
{
    /* ---- Find valid columns -------------------------------- */
    S = 0;
    for (int i = 0; i < 128; ++i) col_index[i] = -1;    /* Initialise mapping */

    for (int col = 0; col < 128; ++col) {
        if (!valid_column((uint8_t)col)) continue;
        if (S >= MAXS) { /* safety */
            /* If more than MAXS come up, abort */
            return;
        }
        valid_cols[S] = (uint8_t)col;
        col_index[col] = S;
        ++S;
    }

    S2 = S * S;

    /* ---- Build transition lists ---------------------------- */
    /* adj_len and adj_list already zeroed by static init */

    for (int a = 0; a < S; ++a) {
        for (int b = 0; b < S; ++b) {
            int pair = a * S + b;
            int len = 0;

            for (int c = 0; c < S; ++c) {
                bool ok = true;
                /* For each row check horizontal constraint 000/111 */
                for (int r = 0; r < 7; ++r) {
                    int bit_a = (valid_cols[a] >> r) & 1;
                    int bit_b = (valid_cols[b] >> r) & 1;
                    if (bit_a == bit_b) {
                        int must = 1 - bit_a;          /* must be opposite to avoid 000/111 */
                        int bit_c = (valid_cols[c] >> r) & 1;
                        if (bit_c != must) { ok = false; break; }
                    }
                }
                if (ok) {
                    adj_list[pair * MAXS + len] = (uint8_t)c;
                    ++len;
                }
            }
            adj_len[pair] = len;
        }
    }

    /* ---- Allocate DP vectors -------------------------------- */
#ifdef __cplusplus
    dp_cur.assign(S2, seq_t(0));
    dp_next.assign(S2, seq_t(0));
#else
    dp_cur = (seq_t*)calloc(S2, sizeof(seq_t));
    dp_next = (seq_t*)calloc(S2, sizeof(seq_t));
#endif

    built = true;
}

/* ------------------------------------------------------------ */
/*   Main sequence function                                      */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!built) build();

    if (n == 0) return 0;          /* undefined for offset 1 */

    if (n == 1) return (seq_t)S;    /* single column */

    if (n == 2) return (seq_t)S * (seq_t)S;  /* any pair of valid columns */

    /* ---- Initialise dp_cur to all 1 for length 2 ---------------- */
#ifdef __cplusplus
    std::fill(dp_cur.begin(), dp_cur.end(), seq_t(1));
#else
    for (int i = 0; i < S2; ++i) dp_cur[i] = 1;
#endif

    seq_t total = 0;

    for (idx_t step = 3; step <= n; ++step) {
        /* reset dp_next */
#ifdef __cplusplus
        std::fill(dp_next.begin(), dp_next.end(), seq_t(0));
#else
        for (int i = 0; i < S2; ++i) dp_next[i] = 0;
#endif

        /* transition */
        for (int p = 0; p < S2; ++p) {
            seq_t val = dp_cur[p];
            if (val == 0) continue;

            int a_idx = p / S;
            int b_idx = p % S;
            int len   = adj_len[p];

            for (int j = 0; j < len; ++j) {
                int c_idx = adj_list[p * MAXS + j];
                int new_index = b_idx * S + c_idx;
                dp_next[new_index] = dp_next[new_index] + val;
            }
        }

        /* sum all ways for the current length */
        total = 0;
        for (int i = 0; i < S2; ++i) total = total + dp_next[i];

        /* swap dp_cur and dp_next */
#ifdef __cplusplus
        dp_cur.swap(dp_next);
#else
        seq_t *tmp = dp_cur; dp_cur = dp_next; dp_next = tmp;
#endif
    }

    return total;
}
