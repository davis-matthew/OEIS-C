/*
 * A207114  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n 0/1 arrays avoiding vertically the triples
 *            0 0 1 and 1 1 0, and horizontally the triples 0 0 0 and 0 1 1.
 *            The algorithm uses transfer‑matrix DP over pairs of columns
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 20, u64 n <= 45, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Number of possible 5‑bit columns */
#define MAX_COLS 32                  /* 5 bits: 0 … 31 */
#define STATE_MAP(x,y) (((x)<<5)|(y))/* encode a pair of columns */

/* State data */
static bool  valid_col[MAX_COLS];                 /* column satisfies vertical rules */
static int   val_cols[MAX_COLS];                  /* list of valid columns */
static int   num_valid;                           /* number of valid columns */

static int   allowed_cnt[MAX_COLS*MAX_COLS];      /* transitions from pair → list length */
static int   allowed_z[MAX_COLS*MAX_COLS][MAX_COLS];/* list of allowed next columns */

/* Dynamic‑programming storage */
static seq_t *dp_cur   = NULL;      /* counts for state after current length   */
static seq_t *dp_next  = NULL;      /* temporary counts for next length        */
static size_t dp_size  = 0;         /* should be MAX_COLS*MAX_COLS = 1024      */
static seq_t *cache    = NULL;      /* cached answers for each n              */
static size_t cache_cap = 0;        /* allocated capacity                      */
static idx_t  max_calc   = 0;       /* maximum n already computed              */
static bool   inited     = false;   /* flag whether constants were set         */

/* Ensure cache large enough for indices up to n (0‑based). */
static void
ensure_cache(idx_t n)
{
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        /* initialise new entries to 0 */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Build the tables used by the DP. */
static void
init(void)
{
    if (inited) return;
    inited = true;

    /* 1. Find all columns that avoid vertical patterns 0 0 1 and 1 1 0. */
    num_valid = 0;
    for (int col = 0; col < MAX_COLS; ++col) {
        bool ok = true;
        for (int r = 0; r <= 2; ++r) {
            int a = (col >> r) & 1;       /* row r   */
            int b = (col >> (r + 1)) & 1; /* row r+1 */
            int c = (col >> (r + 2)) & 1; /* row r+2 */
            if ((a == 0 && b == 0 && c == 1) ||
                (a == 1 && b == 1 && c == 0)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            valid_col[col] = true;
            val_cols[num_valid++] = col;
        } else {
            valid_col[col] = false;
        }
    }

    /* 2. Build transition lists for horizontal patterns 0 0 0 and 0 1 1. */
    for (int x = 0; x < MAX_COLS; ++x) {
        if (!valid_col[x]) continue;
        for (int y = 0; y < MAX_COLS; ++y) {
            if (!valid_col[y]) continue;
            int idx = STATE_MAP(x, y);
            int cnt = 0;
            for (int z = 0; z < MAX_COLS; ++z) {
                if (!valid_col[z]) continue;
                bool ok = true;
                for (int r = 0; r < 5; ++r) {
                    int a = (x >> r) & 1;
                    int b = (y >> r) & 1;
                    int c = (z >> r) & 1;
                    if ((a == 0 && b == 0 && c == 0) ||
                        (a == 0 && b == 1 && c == 1)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    allowed_z[idx][cnt++] = z;
                }
            }
            allowed_cnt[idx] = cnt;
        }
    }

    /* 3. Allocate DP arrays */
    dp_size = MAX_COLS * MAX_COLS;          /* 1024 */
    dp_cur  = (seq_t *)calloc(dp_size, sizeof(seq_t));
    dp_next = (seq_t *)calloc(dp_size, sizeof(seq_t));

    /* 4. Initialise dp_cur for length 2 (no horizontal restriction yet). */
    for (int i = 0; i < num_valid; ++i) {
        int x = val_cols[i];
        for (int j = 0; j < num_valid; ++j) {
            int y = val_cols[j];
            dp_cur[STATE_MAP(x, y)] = (seq_t)1;
        }
    }

    /* 5. Prepare cache array and store first two terms. */
    ensure_cache(2);                     /* indices 0 and 1 are unused */
    cache[1] = (seq_t)num_valid;         /* a(1) */
    /* sum dp_cur to get a(2) */
    seq_t total = 0;
    for (size_t i = 0; i < dp_size; ++i) total += dp_cur[i];
    cache[2] = total;                    /* a(2) */
    max_calc = 2;
}

/* Retrieve the nth term of the sequence. */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;     /* no definition below the offset */

    init();

    /* Return cached value if already computed. */
    if (n <= max_calc) return cache[n];

    /* Extend the DP up to n. */
    ensure_cache(n);

    for (idx_t len = max_calc + 1; len <= n; ++len) {
        /* reset dp_next */
        for (size_t i = 0; i < dp_size; ++i) dp_next[i] = 0;

        /* transition from length len-1 to len */
        for (int s = 0; s < (int)dp_size; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;

            int idx = s;               /* STATE_MAP(x,y) */
            int cnt = allowed_cnt[idx];
            for (int k = 0; k < cnt; ++k) {
                int z = allowed_z[idx][k];
                int t = STATE_MAP(idx & 31, z); /* previous y becomes new x, z new y */
                dp_next[t] += val;
            }
        }

        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur   = dp_next;
        dp_next  = tmp;

        /* compute total for this length */
        seq_t total = 0;
        for (int i = 0; i < (int)dp_size; ++i) total += dp_cur[i];
        cache[len] = total;
    }

    max_calc = n;
    return cache[n];
}
