/*
 * A208116  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 29, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A208116: number of n×6 binary arrays avoiding the patterns
   000 and 010 horizontally, and 001 and 011 vertically.
   Computation is performed by dynamic programming over all valid
   row masks.  Initial values: a(1)=25, a(2)=625. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- global data ---------- */
static bool initialized = false;

/* number of valid row masks (those not containing 000 or 010) */
static int M;

/* mapping from a 6‑bit mask to its index in valid_masks (or -1) */
static int mask_idx[64];

/* list of valid masks */
static int valid_masks[64];

/* trans[i][j] is a bitset: bit k set iff mask_k can follow masks i,j */
static uint32_t trans[25][25];

/* two DP buffers */
static seq_t dp1[25][25], dp2[25][25];

/* pointers to the current and next DP buffers */
static seq_t (*dp_cur)[25] = dp1;
static seq_t (*dp_next)[25] = dp2;

/* cache of already computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored (a(1)...a(cache_len)) */
static size_t cache_cap = 0;   /* capacity of cache array */

/* ---------- helper: initialise state ---------- */
static void init(void)
{
    /* build table of valid masks */
    int cnt = 0;
    for (int m = 0; m < 64; ++m) {
        int ok = 1;
        /* check all horizontal triples in the mask */
        for (int i = 0; i <= 3; ++i) {
            int t = (m >> i) & 0x7;      /* 3‑bit window */
            if (t == 0 || t == 2) {      /* 000 or 010 forbidden */
                ok = 0;
                break;
            }
        }
        mask_idx[m] = ok ? cnt++ : -1;
        if (ok) valid_masks[mask_idx[m]] = m;
    }
    M = cnt;   /* must be 25 */

    /* build transition table */
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            uint32_t bits = 0;
            for (int k = 0; k < M; ++k) {
                int a = valid_masks[i];
                int b = valid_masks[j];
                int c = valid_masks[k];
                int ok = 1;
                for (int col = 0; col < 6; ++col) {
                    int ai = (a >> col) & 1;
                    int bi = (b >> col) & 1;
                    int ci = (c >> col) & 1;
                    /* vertical triple 001 or 011 forbidden */
                    if ((ai == 0 && bi == 0 && ci == 1) ||
                        (ai == 0 && bi == 1 && ci == 1)) {
                        ok = 0;
                        break;
                    }
                }
                if (ok) bits |= (1U << k);
            }
            trans[i][j] = bits;
        }
    }

    /* initialise DP for length 2: each pair of valid masks allowed */
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < M; ++j)
            dp_cur[i][j] = 1;

    /* initialise cache with a(1) and a(2) */
    cache_cap = 16;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache_len = 2;
    cache[0] = (seq_t)M;          /* a(1) */
    cache[1] = (seq_t)M * (seq_t)M; /* a(2) */

    initialized = true;
}

/* ---------- helper: grow cache ---------- */
static void cache_append(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n == 0) return 0;          /* offset is 1 */

    /* use cached values if available */
    if (n <= cache_len)
        return cache[n - 1];

    /* extend cache up to n */
    size_t target = (size_t)n;
    for (size_t len = cache_len + 1; len <= target; ++len) {
        seq_t term;

        /* DP step for len >= 3 */
        /* zero dp_next */
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                dp_next[i][j] = 0;

        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < M; ++j) {
                seq_t cnt = dp_cur[i][j];
                if (!cnt) continue;
                uint32_t bits = trans[i][j];
                while (bits) {
                    unsigned int k = __builtin_ctz(bits);
                    dp_next[j][k] += cnt;
                    bits &= bits - 1;
                }
            }
        }

        /* swap buffers: new data becomes current */
        seq_t (*tmp)[25] = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        /* sum all counts for the current length */
        term = 0;
        for (int i = 0; i < M; ++i)
            for (int j = 0; j < M; ++j)
                term += dp_cur[i][j];

        cache_append(term);
    }

    return cache[n - 1];
}
