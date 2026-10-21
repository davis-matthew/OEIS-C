/*
 * A207439  Number of nX5 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207439 – number of n×5 binary arrays avoiding the patterns
   0 0 1 and 0 1 0 horizontally and 0 0 1 and 1 0 0 vertically.
   Computation is performed by dynamic programming on the last two rows
   of an array.  The algorithm is O(n·14²) and uses small, fixed‐size
   tables.  It works for arbitrary‑precision seq_t as well as for uint64_t. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Helper tables – built once on the first call to a()                 */
/* ------------------------------------------------------------------ */
static int   M_global;                      /* number of admissible rows */
static uint8_t allowed_rows[32];            /* masks of admissible rows   */
static uint8_t next_counts[32 * 32];        /* number of possible next rows */
static uint8_t next_rows[32 * 32][32];      /* list of possible next rows */
static bool initialized = false;

static void init_tables(void)
{
    if (initialized) return;
    initialized = true;

    /* --- Enumerate admissible rows -------------------------------- */
    int cnt = 0;
    for (int mask = 0; mask < 32; ++mask) {
        bool ok = true;
        for (int pos = 0; pos <= 2; ++pos) {
            int a = (mask >> (4 - pos))     & 1;   /* leftmost of the triple */
            int b = (mask >> (4 - (pos + 1))) & 1;
            int c = (mask >> (4 - (pos + 2))) & 1;
            if ((a == 0 && b == 0 && c == 1) ||    /* 0 0 1 */
                (a == 0 && b == 1 && c == 0)) {    /* 0 1 0 */
                ok = false;
                break;
            }
        }
        if (ok) {
            allowed_rows[cnt++] = (uint8_t)mask;
        }
    }
    M_global = cnt;   /* expected to be 14 */

    /* --- Precompute transitions between consecutive row pairs -------- */
    for (int pi = 0; pi < M_global; ++pi) {
        for (int qi = 0; qi < M_global; ++qi) {
            int state = pi * M_global + qi;
            int k = 0;
            for (int ri = 0; ri < M_global; ++ri) {
                bool ok = true;
                for (int col = 0; col < 5; ++col) {
                    int a = (allowed_rows[pi] >> (4 - col)) & 1;
                    int b = (allowed_rows[qi] >> (4 - col)) & 1;
                    int c = (allowed_rows[ri] >> (4 - col)) & 1;
                    if ((a == 0 && b == 0 && c == 1) ||   /* vertically 0 0 1 */
                        (a == 1 && b == 0 && c == 0)) {   /* vertically 1 0 0 */
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    next_rows[state][k++] = (uint8_t)ri;
                }
            }
            next_counts[state] = (uint8_t)k;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_tables();

    /* The sequence is defined for n ≥ 1 */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)M_global;   /* all admissible rows */

    /* dp_cur[state] holds the number of ways to build current
       height rows ending with the pair of rows encoded in 'state'.  */
    seq_t dp_cur[32 * 32];
    seq_t dp_next[32 * 32];
    int states = M_global * M_global;

    /* Height 2: any pair of admissible rows is allowed */
    for (int i = 0; i < states; ++i) dp_cur[i] = 1;

    /* Build rows from height 3 up to height n */
    for (idx_t row = 2; row < n; ++row) {
        /* zero next dp array */
        for (int i = 0; i < states; ++i) dp_next[i] = 0;

        for (int state = 0; state < states; ++state) {
            seq_t val = dp_cur[state];
            if (!val) continue;

            int q = state % M_global;               /* second row of the pair */
            uint8_t *choices = next_rows[state];
            uint8_t  cnt = next_counts[state];

            for (uint8_t k = 0; k < cnt; ++k) {
                int r = choices[k];
                int new_state = q * M_global + r;
                dp_next[new_state] += val;
            }
        }

        /* prepare for next iteration */
        for (int i = 0; i < states; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum over all possible final pairs */
    seq_t total = 0;
    for (int i = 0; i < states; ++i) total += dp_cur[i];
    return total;
}
