/*
 * A188604  Number of n X 6 binary arrays without the pattern 1 1 0
 *          diagonally, vertically, antidiagonally or horizontally.
 * https://oeis.org/A188604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   row_has110
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A188604
    Number of n×6 binary arrays without the pattern 1 1 0
    diagonally, vertically (in a column), anti‑diagonally or
    horizontally (in a row).  This implementation uses a
    transfer‑matrix dynamic programming over consecutive rows.
    The DP state is a pair of 6‑bit rows without horizontal “110”.
    For each state we pre–compute the list of rows that may follow,
    ensuring the vertical, diagonal and anti‑diagonal constraints.
    The answer for n≥3 is obtained by iterating this transition
    and summing all ending states.  The first two terms are handled
    specially, because no 3‑row patterns exist.  All arithmetic uses
    the seq_t type supplied by the build. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/row_has110.h"

#define STATE_ROWS 64                     /* 2^6 possible row patterns */
#define MAX_ROWS   33                     /* actual number of allowed rows */
#define MAX_STATES (MAX_ROWS * MAX_ROWS)  /* maximum number of valid state pairs */

/* --------------------------------------------------------------------- */
/*  Sequence function                                                   */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static tables, built on first call -------------------------------- */
    static bool          initialized = false;

    static bool          allowed[STATE_ROWS];
    static uint8_t       allowed_rows[MAX_ROWS];
    static uint16_t      pair_id[STATE_ROWS * STATE_ROWS]; /* 0xFFFF = invalid */

    /* transitions: trans_to[source_state][k] = destination_state (uint16_t) */
    static uint16_t trans_to[MAX_STATES][MAX_ROWS];
    static uint8_t  trans_cnt[MAX_STATES];

    static seq_t dp_arr[MAX_STATES];
    static seq_t ndp_arr[MAX_STATES];

    /* --------------------------------------------------------------- */

    if (!initialized)
    {
        /* Determine allowed rows (no horizontal “110”) */
        for (int i = 0; i < STATE_ROWS; ++i)
            allowed[i] = !row_has110((uint8_t)i);

        /* Build list of allowed rows and mapping to indices */
        int allowed_cnt = 0;
        for (int i = 0; i < STATE_ROWS; ++i)
            if (allowed[i])
                allowed_rows[allowed_cnt++] = (uint8_t)i;

        /* Mark all pairs as invalid initially */
        for (int i = 0; i < STATE_ROWS * STATE_ROWS; ++i)
            pair_id[i] = 0xFFFFu;

        /* Assign state IDs to all allowed pairs (a,b) */
        int state_cnt = 0;
        for (int a = 0; a < allowed_cnt; ++a)
            for (int b = 0; b < allowed_cnt; ++b)
            {
                int key = (allowed_rows[a] << 6) | allowed_rows[b];
                pair_id[key] = (uint16_t)state_cnt;
                ++state_cnt;
            }

        /* Pre‑compute transitions for every state (a,b) -> list of (b,c) */
        for (int a = 0; a < allowed_cnt; ++a)
        {
            for (int b = 0; b < allowed_cnt; ++b)
            {
                int key_ab = (allowed_rows[a] << 6) | allowed_rows[b];
                uint16_t src_state = pair_id[key_ab];
                uint8_t  tcount = 0;

                for (int c = 0; c < allowed_cnt; ++c)
                {
                    bool bad = false;
                    /* vertical 1 1 0 */
                    for (int col = 0; col < 6; ++col)
                    {
                        int bit_a = (allowed_rows[a] >> col) & 1;
                        int bit_b = (allowed_rows[b] >> col) & 1;
                        int bit_c = (allowed_rows[c] >> col) & 1;
                        if (bit_a && bit_b && !bit_c) { bad = true; break; }
                    }
                    if (bad) continue;
                    /* diagonal 1 1 0 (top‑left → bottom‑right) */
                    for (int col = 0; col <= 3 && !bad; ++col)
                    {
                        int bit_a = (allowed_rows[a] >> col) & 1;
                        int bit_b = (allowed_rows[b] >> (col + 1)) & 1;
                        int bit_c = (allowed_rows[c] >> (col + 2)) & 1;
                        if (bit_a && bit_b && !bit_c) bad = true;
                    }
                    if (bad) continue;
                    /* anti‑diagonal 1 1 0 (top‑right → bottom‑left) */
                    for (int col = 2; col < 6 && !bad; ++col)
                    {
                        int bit_a = (allowed_rows[a] >> col) & 1;
                        int bit_b = (allowed_rows[b] >> (col - 1)) & 1;
                        int bit_c = (allowed_rows[c] >> (col - 2)) & 1;
                        if (bit_a && bit_b && !bit_c) bad = true;
                    }
                    if (bad) continue;

                    /* valid transition: go to (b,c) */
                    int key_bc = (allowed_rows[b] << 6) | allowed_rows[c];
                    uint16_t dst_state = pair_id[key_bc];
                    trans_to[src_state][tcount++] = dst_state;
                }
                trans_cnt[src_state] = tcount;
            }
        }

        initialized = true;
    }

    /* --------------------------------------------------------------- */
    /* Special handling for n = 1, 2 ----------------------------------- */
    if (n == 1) /* any allowed single row */
    {
        seq_t res = 0;
        for (int i = 0; i < STATE_ROWS; ++i)
            if (allowed[i]) res += (seq_t)1;
        return res;
    }
    if (n == 2) /* any ordered pair of allowed rows */
    {
        seq_t cnt = 0;
        for (int i = 0; i < STATE_ROWS; ++i)
            if (allowed[i]) cnt += (seq_t)1;
        return cnt * cnt;
    }

    /* --------------------------------------------------------------- */
    /* DP for n >= 3 --------------------------------------------------- */
    /* Determine state count from mapping table */
    uint16_t state_cnt = 0;
    for (int key = 0; key < STATE_ROWS * STATE_ROWS; ++key)
        if (pair_id[key] != 0xFFFFu && pair_id[key] + 1 > state_cnt)
            state_cnt = pair_id[key] + 1;

    /* initialize dp for two first rows: one for each state */
    for (size_t i = 0; i < state_cnt; ++i)
        dp_arr[i] = (seq_t)1;

    seq_t *dp  = dp_arr;
    seq_t *ndp = ndp_arr;

    /* iterate rows 3 .. n */
    for (uint64_t step = 3; step <= n; ++step)
    {
        for (size_t i = 0; i < state_cnt; ++i)
            ndp[i] = (seq_t)0;

        for (size_t i = 0; i < state_cnt; ++i)
        {
            seq_t val = dp[i];
            if (val == (seq_t)0) continue;
            uint16_t *list = trans_to[i];
            uint8_t   cnt   = trans_cnt[i];
            for (uint8_t t = 0; t < cnt; ++t)
                ndp[list[t]] += val;
        }
        /* swap */
        seq_t *tmp = dp;
        dp = ndp;
        ndp = tmp;
    }

    /* final answer: sum all states */
    seq_t total = 0;
    for (size_t i = 0; i < state_cnt; ++i)
        total += dp[i];
    return total;
}
