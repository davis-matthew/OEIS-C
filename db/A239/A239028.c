/*
 * A239028  Number of nX6 0..2 arrays with no element equal to the sum of
 *          elements to its left or one plus the sum of elements above it,
 *          modulo 3.
 * https://oeis.org/A239028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   state_to_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239028: number of n×6 arrays over {0,1,2}
 * with no entry equal to the sum of earlier entries in its row
 * or one plus the sum of earlier entries in its column,
 * all sums taken modulo 3. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/state_to_digits.h"

/* Pre‑computed powers of 3 for state encoding. */
static const unsigned char MULT[6] = {243, 81, 27, 9, 3, 1};

/* 3^6 possible column‑sum states (mod 3). */
#define STATE_COUNT 729
/* Upper bound on legal rows coming from any state. 64 = 2^6. */
#define MAX_LEGAL_ROWS 64

/* Transition tables.  trans[s][k] = destination state from state s,
 *   trans_len[s] = number of legal rows from state s.  Initialized once
 *   on first call to a(). */
static uint16_t trans[STATE_COUNT][MAX_LEGAL_ROWS];
static uint8_t  trans_len[STATE_COUNT];
static bool     trans_built = false;

/* Build the transition tables on first use. */
static void build_transitions(void)
{
    if (trans_built) return;

    /* Pre‑compute the ternary digits of all 729 possible rows. */
    static unsigned char row_digits[STATE_COUNT][6];
    for (uint16_t r = 0; r < STATE_COUNT; ++r)
        state_to_digits(r, row_digits[r]);

    for (uint16_t src = 0; src < STATE_COUNT; ++src) {
        unsigned char src_d[6];
        state_to_digits(src, src_d);

        unsigned char left_sum = 0;  /* reused below only inside inner loop */
        uint8_t len = 0;

        for (uint16_t r = 0; r < STATE_COUNT; ++r) {
            unsigned char *row = row_digits[r];
            unsigned char forb1, forb2;
            left_sum = 0;
            bool ok = true;

            /* Check legality of this row with respect to the constraints. */
            for (int j = 0; j < 6 && ok; ++j) {
                forb1 = left_sum;                 /* sum of elements to the left (mod 3) */
                forb2 = (src_d[j] + 1) % 3;        /* one plus sum of elements above (mod 3) */
                if (row[j] == forb1 || row[j] == forb2)
                    ok = false;
                left_sum = (left_sum + row[j]) % 3;
            }
            if (!ok) continue;

            /* Compute destination state after adding this row. */
            unsigned char dest[6];
            unsigned int dest_idx = 0;
            for (int j = 0; j < 6; ++j) {
                dest[j] = (src_d[j] + row[j]) % 3;
                dest_idx += dest[j] * MULT[j];
            }

            trans[src][len++] = (uint16_t)dest_idx;
        }
        trans_len[src] = len;
    }

    trans_built = true;
}

/* Count arrays of size n×6 satisfying the conditions. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* safety */

    build_transitions();

    /* Two DP buffers holding counts for each state. */
    seq_t dp[STATE_COUNT];
    seq_t newdp[STATE_COUNT];

    for (int i = 0; i < STATE_COUNT; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;  /* starting column sums are all zero */

    seq_t *cur  = dp;
    seq_t *next = newdp;

    for (idx_t row = 1; row <= n; ++row) {
        /* clear next buffer */
        for (int i = 0; i < STATE_COUNT; ++i) next[i] = (seq_t)0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = cur[s];
            if (val == (seq_t)0) continue;

            uint8_t  len = trans_len[s];
            uint16_t *dst = trans[s];

            for (uint8_t k = 0; k < len; ++k)
                next[dst[k]] += val;
        }

        /* swap buffers */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* sum counts over all final states */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATE_COUNT; ++i) total += cur[i];
    return total;
}
