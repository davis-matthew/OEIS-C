/*
 * A212723  Number of 0..2 arrays of length n+3 with sum less than 4 in any
 *          length 4 subsequence (=less than 50% duty cycle).
 * https://oeis.org/A212723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0..2 arrays of length n+3 with sum <4 in any *
 *            length‑4 subsequence. Computed by dynamic programming on
 *            3‑tuples
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 25, u64 n <= 54, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ------------------------------------------------------------
   State machine for the last three elements of a sequence.
   There are 3^3 = 27 states, encoded as a = 0..2,
   b = 0..2, c = 0..2  →  index = a + 3*b + 9*c.
   ------------------------------------------------------------ */
#define STATE_COUNT 27

static int next_state[STATE_COUNT][3];
static int sum_last[STATE_COUNT];

/* DP tables: dp_cur[s] = number of sequences of current length that end
 * in state s.  Initially the length is 3 (any 3‑tuple allowed).
 */
static seq_t dp_cur[STATE_COUNT];
static seq_t dp_next[STATE_COUNT];

/* Cache of a(n) values.  a_vals[1] = a(1).  Index 0 is unused. */
static seq_t *a_vals = NULL;
static size_t a_vals_cap = 0;
static size_t a_vals_used = 0;   /* maximum n that has been computed */
static int table_built = 0;

/* ------------------------------------------------------------
   Build the transition tables once.
   ------------------------------------------------------------ */
static void build_tables(void)
{
    int idx = 0;
    for (int a = 0; a < 3; ++a) {
        for (int b = 0; b < 3; ++b) {
            for (int c = 0; c < 3; ++c, ++idx) {
                int s = a + 3 * b + 9 * c;
                sum_last[s] = a + b + c;
                for (int d = 0; d < 3; ++d) {
                    if (sum_last[s] + d <= 3) {
                        int ns = b + 3 * c + 9 * d;  /* shift window: (b,c,d) */
                        next_state[s][d] = ns;
                    } else {
                        next_state[s][d] = -1;
                    }
                }
            }
        }
    }

    /* Initial DP tables for length 3: every 3‑tuple is possible. */
    for (int s = 0; s < STATE_COUNT; ++s)
        dp_cur[s] = 1;
}

/* ------------------------------------------------------------
   Extend the cache up to at least n (n ≥ 1).
   ------------------------------------------------------------ */
static void extend_cache(idx_t n)
{
    if (!table_built) {
        build_tables();
        table_built = 1;
    }

    if ((size_t)n <= a_vals_used)
        return;   /* already computed */

    /* Allocate or grow the cache array if necessary. */
    if (a_vals_used == 0) {
        a_vals_cap = 64;
        a_vals = (seq_t *)malloc((a_vals_cap + 1) * sizeof(seq_t));
        if (!a_vals) abort();
    }
    if ((size_t)n >= a_vals_cap) {
        while ((size_t)n >= a_vals_cap)
            a_vals_cap <<= 1;
        a_vals = (seq_t *)realloc(a_vals, (a_vals_cap + 1) * sizeof(seq_t));
        if (!a_vals) abort();
    }

    /* Compute values for all n' in (a_vals_used+1) … n. */
    for (size_t cur = a_vals_used + 1; cur <= (size_t)n; ++cur) {
        /* One DP step: increase the length by 1. */
        for (int s = 0; s < STATE_COUNT; ++s)
            dp_next[s] = 0;
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            for (int d = 0; d < 3; ++d) {
                int ns = next_state[s][d];
                if (ns >= 0)
                    dp_next[ns] += val;
            }
        }
        for (int s = 0; s < STATE_COUNT; ++s)
            dp_cur[s] = dp_next[s];

        /* Sum all states to obtain a(cur).  a(1) corresponds to length 4. */
        seq_t total = 0;
        for (int s = 0; s < STATE_COUNT; ++s)
            total += dp_cur[s];
        a_vals[cur] = total;
    }

    a_vals_used = n;
}

/* ------------------------------------------------------------
   Public entry point.
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for offset 1; return 0 safely */
        return 0;
    extend_cache(n);
    return a_vals[n];
}
