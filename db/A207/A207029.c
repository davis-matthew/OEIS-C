/*
 * A207029  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n binary arrays avoiding * – horizontally the
 *            patterns 000 and 011 in every row, * – vertically the patterns
 *            001 and 101 in every column. * The implementation uses a
 *            dynamic‑programming approach over column * pairs. It precomputes
 *            the vertical validity of each column and * the...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 20, u64 n <= 53, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Guard for the (unused) offset 0; a(1) is defined. */
    if (n == 0) return 0;

    /* --------------------------------------------------------------- */
    /* Tables and state shared across calls.                           */
    /* --------------------------------------------------------------- */
    static bool            initialized = false;
    static seq_t          *cache       = NULL;   /* a[0] unused, a[1]… */
    static idx_t           cache_size  = 0;     /* highest index stored */
    static seq_t            curr[4096];
    static seq_t            next[4096];
    static uint32_t       *next_offset = NULL;  /* start index of transitions */
    static uint16_t       *next_count  = NULL;  /* number of transitions per state */
    static int            *next_list   = NULL;  /* flat list of new states */
    static uint8_t          valid_col[64];
    static idx_t           current_step = 0;    /* length of curr array */

    /* --------------------------------------------------------------- */
    /* Initialise once: pre‑compute vertical validity, transitions,  */
    /* and the first two terms a(1) and a(2).                          */
    /* --------------------------------------------------------------- */
    if (!initialized) {
        /* 1. Vertical constraints: a column is valid iff it does not
         * contain 001 or 101 anywhere in its 6 cells. */
        for (int c = 0; c < 64; ++c) {
            int ok = 1;
            for (int r = 0; r <= 3; ++r) {
                int b0 = (c >> r)     & 1;
                int b1 = (c >> (r+1)) & 1;
                int b2 = (c >> (r+2)) & 1;
                if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                    (b0 == 1 && b1 == 0 && b2 == 1)) {
                    ok = 0;
                    break;
                }
            }
            valid_col[c] = (uint8_t)ok;
        }

        /* 2. Count of valid single columns = a(1). */
        idx_t cnt_valid_cols = 0;
        for (int c = 0; c < 64; ++c)
            if (valid_col[c]) ++cnt_valid_cols;

        /* 3. Pre‑compute transitions between column pairs. */
        /*    state = (prev_col << 6) | curr_col  (6 bits each).  */
        static uint16_t  transitions_per_state[4096];
        size_t total_edges = 0;
        for (int s = 0; s < 4096; ++s) {
            int a_col = s >> 6;
            int b_col = s & 63;
            if (!valid_col[a_col] || !valid_col[b_col]) {
                transitions_per_state[s] = 0;
                continue;
            }
            uint16_t cnt = 0;
            for (int c_col = 0; c_col < 64; ++c_col) {
                if (!valid_col[c_col]) continue;
                /* Check horizontal patterns 000 and 011 for all rows. */
                int ok = 1;
                for (int r = 0; r < 6; ++r) {
                    int x = (a_col >> r) & 1;
                    int y = (b_col >> r) & 1;
                    int z = (c_col >> r) & 1;
                    if ((x == 0 && y == 0 && z == 0) ||
                        (x == 0 && y == 1 && z == 1)) {
                        ok = 0;
                        break;
                    }
                }
                if (ok) ++cnt;
            }
            transitions_per_state[s] = cnt;
            total_edges += cnt;
        }

        next_offset = (uint32_t*)malloc(4096 * sizeof(uint32_t));
        next_count  = (uint16_t*) malloc(4096 * sizeof(uint16_t));
        next_list   = (int*)    malloc(total_edges * sizeof(int));

        /* Set offsets and fill the flat list of transitions. */
        uint32_t acc = 0;
        for (int s = 0; s < 4096; ++s) {
            next_offset[s] = acc;
            next_count[s]  = transitions_per_state[s];
            acc += transitions_per_state[s];
        }

        uint32_t write_pos = 0;
        for (int s = 0; s < 4096; ++s) {
            int a_col = s >> 6;
            int b_col = s & 63;
            if (!valid_col[a_col] || !valid_col[b_col]) continue;
            for (int c_col = 0; c_col < 64; ++c_col) {
                if (!valid_col[c_col]) continue;
                int ok = 1;
                for (int r = 0; r < 6; ++r) {
                    int x = (a_col >> r) & 1;
                    int y = (b_col >> r) & 1;
                    int z = (c_col >> r) & 1;
                    if ((x == 0 && y == 0 && z == 0) ||
                        (x == 0 && y == 1 && z == 1)) {
                        ok = 0;
                        break;
                    }
                }
                if (ok) {
                    next_list[write_pos++] = (b_col << 6) | c_col;
                }
            }
        }

        /* 4. Initialise cache for n=1 and n=2. */
        cache_size   = 2;
        cache  = (seq_t*)calloc(cache_size + 1, sizeof(seq_t)); /* cache[0] unused */
        cache[1] = (seq_t)cnt_valid_cols;                       /* a(1) */
        cache[2] = 0;                                           /* will compute below */

        /* DP for length 2 (no horizontal constraints exist). */
        for (int s = 0; s < 4096; ++s) {
            int a_col = s >> 6;
            int b_col = s & 63;
            curr[s] = (valid_col[a_col] && valid_col[b_col]) ? 1 : 0;
        }
        seq_t sum = 0;
        for (int s = 0; s < 4096; ++s) sum += curr[s];
        cache[2] = sum;
        current_step = 2;

        initialized = true;
    }

    /* --------------------------------------------------------------- */
    /* Extend the cache up to the requested n if needed.               */
    /* --------------------------------------------------------------- */
    if (n > cache_size) {
        /* Allocate a larger cache. */
        seq_t *new_cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        memcpy(new_cache, cache, (cache_size + 1) * sizeof(seq_t));
        memset(new_cache + cache_size + 1, 0, (n - cache_size) * sizeof(seq_t));
        free(cache);
        cache     = new_cache;
        cache_size = n;

        /* Extend DP from current_step + 1 up to n. */
        for (idx_t k = current_step + 1; k <= n; ++k) {
            memset(next, 0, 4096 * sizeof(seq_t));

            for (int s = 0; s < 4096; ++s) {
                seq_t v = curr[s];
                if (!v) continue;
                uint32_t off  = next_offset[s];
                uint16_t cnt  = next_count[s];
                for (uint16_t t = 0; t < cnt; ++t) {
                    int new_state = next_list[off + t];
                    next[new_state] += v;
                }
            }

            /* Sum all possible last states to obtain a(k). */
            seq_t total = 0;
            for (int s = 0; s < 4096; ++s) total += next[s];
            cache[k] = total;

            /* Prepare for next iteration. */
            memcpy(curr, next, 4096 * sizeof(seq_t));
            current_step = k;
        }
    }

    return cache[n];
}
