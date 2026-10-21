/*
 * A200167  Number of -n..n arrays x(0..3) of 4 elements with nonzero sum and
 *          with zero through 3 differences all nonzero.
 * https://oeis.org/A200167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) +2*a(n-3) -a(n-4) -3*a(n-5) -a(n-6) +a(n-9)
 *            +3*a(n-10) +a(n-11) -2*a(n-12) -2*a(n-13) +a(n-15)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 128, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ----------------------------------------------------------------- */
/*  Compute the first 15 terms directly by brute force               */
/* ----------------------------------------------------------------- */
static seq_t init_terms[16];
static bool init_done = false;

/* helper: count valid arrays for a given n (n <= 15) */
static seq_t count_valid(idx_t n) {
    int limit = (int)n;
    int vals[31];
    int m = 0;
    for (int v = -limit; v <= limit; ++v)
        if (v != 0)
            vals[m++] = v;

    seq_t cnt = 0;
    for (int i = 0; i < m; ++i) {
        int x0 = vals[i];
        for (int j = 0; j < m; ++j) {
            int x1 = vals[j];
            if (x1 - x0 == 0) continue;
            for (int k = 0; k < m; ++k) {
                int x2 = vals[k];
                if (x2 - x1 == 0) continue;
                if (x2 - 2 * x1 + x0 == 0) continue;
                for (int l = 0; l < m; ++l) {
                    int x3 = vals[l];
                    if (x3 - x2 == 0) continue;
                    if (x3 - 2 * x2 + x1 == 0) continue;
                    if (x3 - 3 * x2 + 3 * x1 - x0 == 0) continue;
                    seq_t sum = (seq_t)x0 + x1 + x2 + x3;
                    if (sum == 0) continue;
                    ++cnt;
                }
            }
        }
    }
    return cnt;
}

static void init_sequence(void) {
    if (init_done) return;
    init_terms[1] = 0; /* provided */
    for (idx_t i = 2; i <= 15; ++i)
        init_terms[i] = count_valid(i);
    init_done = true;
}

/* ----------------------------------------------------------------- */
/*  Memoised dynamic array of computed values                         */
/* ----------------------------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static idx_t cache_len = 0;   /* one past last written index */

/* helper: grow cache to hold at least n+1 entries */
static void ensure_cache(idx_t n) {
    size_t needed = (size_t)n + 1;
    if (cache_capacity >= needed) return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort(); /* allocation failure */
    cache = new_ptr;
    cache_capacity = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not part of the sequence */
    init_sequence();
    ensure_cache(n);

    /* compute missing values up to n */
    if (cache_len <= n) {
        /* fill base values up to 15 */
        for (idx_t i = cache_len; i <= n && i <= 15; ++i)
            cache[i] = init_terms[i];
        /* recurrence for n >= 16 */
        for (idx_t i = (cache_len > 15 ? cache_len : 16); i <= n; ++i) {
            seq_t t = (seq_t)2 * cache[i - 2] + (seq_t)2 * cache[i - 3];
            t -= cache[i - 4];
            t -= (seq_t)3 * cache[i - 5];
            t -= cache[i - 6];
            t += cache[i - 9];
            t += (seq_t)3 * cache[i - 10];
            t += cache[i - 11];
            t -= (seq_t)2 * cache[i - 12];
            t -= (seq_t)2 * cache[i - 13];
            t += cache[i - 15];
            cache[i] = t;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
