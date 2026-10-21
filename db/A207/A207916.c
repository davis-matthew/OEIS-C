/*
 * A207916  Number of nX6 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=19, a(2)=361.  
   For n≥3 let V be the 19 0..1 rows of length 6 that avoid the substrings
   000 and 101.  For each pair (r_prev,r_curr)∈V×V we precompute the set of
   r_next∈V that together with r_prev and r_curr do not contain the vertical
   forbidden patterns 010 or 101 in any column.  Then the number of n×6
   arrays equals the number of walks of length n−2 on the directed graph
   whose vertices are the 361 ordered pairs (r_prev,r_curr) with an edge
   from (r_prev,r_curr) to (r_curr,r_next) for every approved r_next.  The
   count for n is obtained by initializing all 361 vertices with weight 1
   (the case n=2) and iterating the weight‑transfer n−3 times. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial terms */
    if (n == 1) return (seq_t)19;
    if (n == 2) return (seq_t)361;

    /* Precomputation of the 19 valid rows and transition table.
       The initialization is done only once using a static flag. */
    enum { ROWS = 19, PAIRS = ROWS * ROWS };
    static bool initialized = false;
    static uint8_t valid_rows[ROWS];
    static int row_index[64];
    static idx_t next_states[PAIRS][ROWS];
    static idx_t next_count[PAIRS];

    if (!initialized) {
        /* Find all rows that avoid 000 and 101. */
        int vi = 0;
        for (idx_t v = 0; v < 64; ++v) {
            bool ok = true;
            for (idx_t i = 0; i <= 3; ++i) {
                idx_t sub = (v >> i) & 7;          /* 3‑bit window */
                if (sub == 0 || sub == 5)          /* 000 or 101 */
                { ok = false; break; }
            }
            if (ok) {
                valid_rows[vi] = (uint8_t)v;
                row_index[v] = vi;
                ++vi;
            } else {
                row_index[v] = -1;
            }
        }
        /* Transition table: for each pair (p,q) list possible r */
        for (idx_t p = 0; p < ROWS; ++p)
            for (idx_t q = 0; q < ROWS; ++q) {
                idx_t pair = p * ROWS + q;
                next_count[pair] = 0;
                for (idx_t r = 0; r < ROWS; ++r) {
                    bool good = true;
                    for (idx_t col = 0; col < 6; ++col) {
                        idx_t a = (valid_rows[p] >> col) & 1;
                        idx_t b = (valid_rows[q] >> col) & 1;
                        idx_t c = (valid_rows[r] >> col) & 1;
                        if ((a == 0 && b == 1 && c == 0) ||
                            (a == 1 && b == 0 && c == 1)) {
                            good = false; break;
                        }
                    }
                    if (good) {
                        idx_t new_pair = q * ROWS + r;
                        next_states[pair][next_count[pair]++] = new_pair;
                    }
                }
            }
        initialized = true;
    }

    /* DP over rows.  dp_cur holds counts for sequences of current length.
       dp_next will accumulate the next step. */
    seq_t dp_cur[PAIRS];
    seq_t dp_next[PAIRS];

    /* Initial vector for length 2: all pairs (p,q) have weight 1. */
    for (idx_t i = 0; i < PAIRS; ++i)
        dp_cur[i] = (seq_t)1;

    /* Iterate from length 3 up to n. */
    for (idx_t step = 3; step <= n; ++step) {
        /* zero next array */
        for (idx_t i = 0; i < PAIRS; ++i)
            dp_next[i] = (seq_t)0;

        /* Transfer weights along allowed edges. */
        for (idx_t i = 0; i < PAIRS; ++i) {
            seq_t val = dp_cur[i];
            if (val == 0) continue;
            idx_t cnt = next_count[i];
            for (idx_t k = 0; k < cnt; ++k) {
                idx_t ns = next_states[i][k];
                dp_next[ns] += val;
            }
        }

        /* Move next to current for next iteration. */
        for (idx_t i = 0; i < PAIRS; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* Sum over all states of length n. */
    seq_t total = (seq_t)0;
    for (idx_t i = 0; i < PAIRS; ++i)
        total += dp_cur[i];

    return total;
}
