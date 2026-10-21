/*
 * A207246  Number of 6 X n 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of 6×n binary arrays avoiding patterns 000
 *            and 011 horizontally and patterns 001 and 010 vertically.
 *            Computed by dynamic programming over pairs of columns
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 15, u64 n <= 41, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Test if a 6‑bit column satisfies the vertical restrictions. */
static inline bool vertical_ok(int col)
{
    for (int r = 0; r <= 3; ++r) {
        int b0 = (col >> r)       & 1;
        int b1 = (col >> (r + 1)) & 1;
        int b2 = (col >> (r + 2)) & 1;
        if ((b0 == 0 && b1 == 0 && b2 == 1) ||
            (b0 == 0 && b1 == 1 && b2 == 0))
            return false;
    }
    return true;
}

/* Test if the horizontal restriction holds for three consecutive columns. */
static inline bool horizontal_ok(int a, int b, int c)
{
    for (int r = 0; r < 6; ++r) {
        int x = (a >> r) & 1;
        int y = (b >> r) & 1;
        int z = (c >> r) & 1;
        if ((x == 0 && y == 0 && z == 0) ||
            (x == 0 && y == 1 && z == 1))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* ----------  Static data, initialized once  ---------- */
    static bool init_done = false;

    static int V[64];            /* valid columns */
    static int Vcnt;             /* number of valid columns */
    static int pair_count;       /* Vcnt * Vcnt */

    static seq_t *dp_prev = NULL; /* counts for current length (last 2 cols) */
    static seq_t *dp_next = NULL; /* buffer for next length                */

    static seq_t *cache = NULL;   /* cached sequence values (a(1)..a(k)) */
    static size_t cache_len = 0;  /* highest n whose value is cached */

    /* ----------------------------------------------------- */
    if (!init_done) {
        /* build list of columns that satisfy vertical constraints */
        Vcnt = 0;
        for (int col = 0; col < 64; ++col)
            if (vertical_ok(col))
                V[Vcnt++] = col;

        pair_count = Vcnt * Vcnt;

        /* allocate DP buffers */
        dp_prev = (seq_t *)malloc(pair_count * sizeof(seq_t));
        dp_next = (seq_t *)malloc(pair_count * sizeof(seq_t));

        /* initialise for length 2 */
        seq_t one = 1;
        for (int i = 0; i < pair_count; ++i)
            dp_prev[i] = one;            /* each pair occurs once */

        /* cache space for a(1) and a(2) */
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)Vcnt;              /* a(1) */
        cache[1] = (seq_t)Vcnt * ((seq_t)Vcnt); /* a(2) */

        init_done = true;
    }

    /* ----------------------------------------------------- */
    /* Grow cache if requested n is beyond current cache_len */
    if (n > cache_len) {
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        /* initialise new entries to 0 for safety */
        for (size_t i = cache_len; i < (size_t)n; ++i)
            cache[i] = 0;
    }

    /* if we already have the value, return it immediately */
    if (n <= cache_len)
        return cache[n - 1];

    /* ----------------------------------------------------- */
    /* Extend computation up to requested n */
    size_t cur_len = cache_len; /* cur_len is the length already computed (>=2) */
    /* dp_prev holds counts for strings of length cur_len (ending in pairs) */

    seq_t zero = 0;  /* used for initialization */

    for (size_t len = cur_len + 1; len <= (size_t)n; ++len) {
        /* zero dp_next */
        for (int i = 0; i < pair_count; ++i)
            dp_next[i] = zero;

        /* transition from dp_prev to dp_next */
        for (int id = 0; id < pair_count; ++id) {
            seq_t val = dp_prev[id];
            if (val == 0) continue;

            int i = id / Vcnt;      /* previous column */
            int j = id % Vcnt;      /* current column  */

            int col_i = V[i];
            int col_j = V[j];

            for (int k = 0; k < Vcnt; ++k) {
                int col_k = V[k];
                if (!horizontal_ok(col_i, col_j, col_k))
                    continue;
                int new_id = j * Vcnt + k;
                dp_next[new_id] += val;
            }
        }

        /* total count for this length and store in cache */
        seq_t total = 0;
        for (int i = 0; i < pair_count; ++i) {
            total += dp_next[i];
            dp_prev[i] = dp_next[i];  /* prepare for next iteration */
        }
        cache[len - 1] = total;
    }

    cache_len = n;
    return cache[n - 1];
}
