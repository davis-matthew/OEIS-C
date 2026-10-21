/*
 * A207749  Number of nX5 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence: A207749
   Number of n×5 binary arrays avoiding horizontal patterns 001,011
   and vertical patterns 001,100.
   Computed by dynamic programming over rows. */
 
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Forward declaration of the equivalent of seq_alloc_new_array:
   returns a pointer to an array of seq_t of length n, zero‑initialized */
static seq_t *seq_alloc_new_array(size_t n)
{
    if (n == 0) return NULL;
    seq_t *p = (seq_t*)calloc(n, sizeof(seq_t));
    return p;
}

static int V = 0;                    /* number of valid rows (12) */
static int validRows[12] = {0};      /* list of valid row masks */
static bool allowed[12][12][12] = {{0}};   /* vertical constraints */

/* DP arrays for pairs of consecutive rows */
static seq_t *dp_cur = NULL;     /* dp for rows i-2,i-1  */
static seq_t *dp_next = NULL;    /* dp for rows i-1,i  */

/* Cache of computed a(n) values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;      /* capacity of cache */
static idx_t cached_n = 0;        /* largest n computed */

/* One‑time initialization */
static void init(void)
{
    /* Find all valid 5‑bit rows (avoid horizontal 001,011) */
    int cnt = 0;
    for (int r = 0; r < 32; ++r) {
        bool ok = true;
        for (int s = 0; s <= 2; ++s) {
            int bits = (r >> s) & 7;  /* 3‑bit window */
            if (bits == 1 || bits == 3) { /* 001 or 011 */
                ok = false;
                break;
            }
        }
        if (ok) validRows[cnt++] = r;
    }
    V = cnt;   /* should be 12 */

    /* Pre‑compute vertical constraints for triples */
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            for (int k = 0; k < V; ++k) {
                int v1 = validRows[i];
                int v2 = validRows[j];
                int v3 = validRows[k];
                bool ok = true;
                for (int c = 0; c < 5; ++c) {
                    int tr = (((v1 >> c) & 1) << 2) |
                              (((v2 >> c) & 1) << 1) |
                              ((v3 >> c) & 1);
                    if (tr == 1 || tr == 4) { /* 001 or 100 */
                        ok = false;
                        break;
                    }
                }
                allowed[i][j][k] = ok;
            }
        }
    }

    /* Allocate DP arrays */
    size_t sz = (size_t)V * (size_t)V;
    dp_cur   = seq_alloc_new_array(sz);
    dp_next  = seq_alloc_new_array(sz);

    /* Initialise cache */
    cache_cap = 0;
    cached_n  = 0;
    free(cache);
    cache = NULL;
}

/* Ensure cache has room for 'needed' entries (1‑based index) */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* allocation failure */
    /* zero any newly allocated entries */
    if (new_cap > cache_cap) memset(tmp + cache_cap, 0,
                                    (new_cap - cache_cap) * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 0, but safe fallback */

    /* Initialise on first call */
    if (cached_n == 0) init();

    /* Return cached value if available */
    if (n <= cached_n) return cache[n - 1];

    /* Compute values up to n incrementally */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        if (i == 1) {
            /* a(1) = number of valid rows */
            ensure_cache_capacity(1);
            cache[0] = (seq_t)V;
            cached_n = 1;
        } else if (i == 2) {
            /* a(2) = V^2, set dp_cur for all pairs */
            size_t sz = (size_t)V * (size_t)V;
            for (size_t p = 0; p < sz; ++p) dp_cur[p] = 1;
            ensure_cache_capacity(2);
            cache[1] = (seq_t)V * (seq_t)V;
            cached_n = 2;
        } else {
            size_t sz = (size_t)V * (size_t)V;
            /* zero dp_next */
            memset(dp_next, 0, sz * sizeof(seq_t));

            /* Transition from rows i-2,i-1 to i-1,i */
            for (int p2 = 0; p2 < V; ++p2) {
                for (int p1 = 0; p1 < V; ++p1) {
                    seq_t cnt = dp_cur[p2 * V + p1];
                    if (!cnt) continue;
                    for (int p3 = 0; p3 < V; ++p3) {
                        if (!allowed[p2][p1][p3]) continue;
                        dp_next[p1 * V + p3] += cnt;
                    }
                }
            }

            /* Swap arrays */
            seq_t *tmp = dp_cur;
            dp_cur = dp_next;
            dp_next = tmp;

            /* Sum all dp_cur entries to get a(i) */
            seq_t sum = 0;
            for (size_t p = 0; p < sz; ++p) sum += dp_cur[p];

            ensure_cache_capacity((size_t)i);
            cache[i - 1] = sum;
            cached_n = i;
        }
    }

    return cache[n - 1];
}
