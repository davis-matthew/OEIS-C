/*
 * A222395  Number of nX1 0..2 arrays with exactly floor(nX1/2) elements
 *          unequal to at least one king-move neighbor, with new values
 *          introduced in row major 0..2 order.
 * https://oeis.org/A222395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×1 arrays over {0,1,2} with first element 0,
 *            respecting the “0 → 1 → 2” introduction order, and with exactly
 *            ⌊n/2⌋ positions that differ from at least one king‑move neighbor
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 31, u64 n <= 57, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* idx_t and seq_t are defined by the build; we only use them as opaque types. */

static inline int state_index(int prev, int flag, int seen) {
    /* prev (0..2), flag (0/1), seen (0/1)  -> unique index 0..11 */
    return (prev << 2) | (flag << 1) | seen;
}

/* Main sequence function ---------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;               /* undefined for n=0, but safe */

    /* n == 1 special case: only the array [0] works */
    if (n == 1) return 1;

    unsigned long long target = n / 2;   /* floor(n/2) */
    size_t  dim = (size_t)(target + 1); /* number of count states */

    /* Allocate two DP layers */
    size_t layer_size = 12 * dim;
    seq_t *cur   = (seq_t *)calloc(layer_size, sizeof(seq_t));
    seq_t *next  = (seq_t *)calloc(layer_size, sizeof(seq_t));
    if (!cur || !next) return 0;        /* safety in case of allocation failure */

    /* ---------- Initialization for position 2 -------------------------------- */
    for (int d = 0; d <= 1; ++d) {      /* 2 cannot appear before the first 1 */
        int seen = (d == 1);            /* seen 1 flag */
        int flag = (d == 0);            /* prevprev == prev (both 0) */
        int prev = d;                   /* current symbol at position 2 */

        int count1 = (d != 0);          /* counted status of position 1 */
        if ((unsigned long long)count1 > target) continue;
        int idx = state_index(prev, flag, seen);
        cur[idx * dim + count1] += 1;
    }

    /* ---------- DP over positions 3 .. n ----------------------------------- */
    for (idx_t pos = 3; pos <= n; ++pos) {
        /* clear next layer */
        memset(next, 0, layer_size * sizeof(seq_t));

        for (int state = 0; state < 12; ++state) {
            int prev  = (state >> 2) & 3;
            int flag  = (state >> 1) & 1;
            int seen  = state & 1;

            seq_t *cur_row = cur + state * dim;

            for (size_t c = 0; c < dim; ++c) {
                seq_t ways = cur_row[c];
                if (!ways) continue;

                /* iterate allowed next symbols */
                for (int d = 0; d <= 2; ++d) {
                    if (!seen && d == 2) continue;          /* 2 before 1 not allowed */
                    int delta = (flag == 0 || d != prev) ? 1 : 0;
                    size_t nc = c + delta;
                    if (nc >= dim) continue;

                    int new_seen = seen | (d == 1);
                    int new_flag = (prev == d);
                    int new_prev = d;

                    int new_state = state_index(new_prev, new_flag, new_seen);
                    next[new_state * dim + nc] += ways;
                }
            }
        }
        /* swap layers */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* ---------- Final counting for position n ------------------------------ */
    seq_t result = 0;
    for (int state = 0; state < 12; ++state) {
        int flag = (state >> 1) & 1;          /* prevprev == prev ? flag */
        seq_t *row = cur + state * dim;

        for (size_t c = 0; c < dim; ++c) {
            seq_t ways = row[c];
            if (!ways) continue;

            int counted_last = (flag == 0);   /* prevprev != prev? */
            unsigned long long total_count = c + counted_last;
            if (total_count == target)
                result += ways;
        }
    }

    free(cur);
    free(next);
    return result;
}
