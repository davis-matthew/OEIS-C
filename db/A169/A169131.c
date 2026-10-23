/*
 * A169131  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^26 = I.
 * https://oeis.org/A169131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts reduced words of length n in the Coxeter group * on
 *            22 generators S_i with relations S_i^2=I and (S_i S_j)^26=I. *
 *            The values are computed by dynamic programming on an automaton *
 *            that tracks the last two distinct generators and the length of
 *            the * current alternating run between them...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* ---------- constants ---------- */
#define NUM_GENS 22          /* number of generators S_i  */
#define KMAX     26          /* maximal allowed alternating run length (0..25) */

/* ---------- state indexing ---------- */
static inline size_t state_idx(int prev, int cur, int k)
{
    /* prev,cur in [0,NUM_GENS-1], prev != cur, k in [0,KMAX-1] */
    return ((prev * NUM_GENS + cur) * KMAX + k);
}

/* ---------- static storage ---------- */
static seq_t *cache      = NULL;   /* a[0..] values                */
static size_t cache_cap  = 0;      /* allocated capacity of cache  */
static seq_t *dp_curr    = NULL;   /* DP array for current length   */
static seq_t *dp_next    = NULL;   /* DP array for next length      */
static idx_t  max_len    = 1;      /* highest length that has been
                                      computed and stored in cache   */
static const size_t STATE_COUNT = NUM_GENS * NUM_GENS * KMAX;

/* ---------- cache handling ---------- */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* allocate room for at least n+2 entries (a(0) .. a(n)) */
        cache_cap = (size_t)(n + 4);
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        cache[0] = (seq_t)1;               /* a(0) = 1 */
        cache[1] = (seq_t)NUM_GENS;        /* a(1) = 22 */
    } else if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* new elements are zero-initialised by realloc? */
        cache_cap = new_cap;
    }
}

/* ---------- DP expansion ---------- */
static void expand_to(idx_t n)
{
    if (n <= max_len) return;            /* already computed */

    ensure_cache(n);

    /* initialise DP arrays on first call for n>=2 */
    if (dp_curr == NULL) {
        dp_curr = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));
        dp_next = (seq_t *)calloc(STATE_COUNT, sizeof(seq_t));

        /* length 2: all ordered pairs of distinct generators */
        for (int i = 0; i < NUM_GENS; ++i) {
            for (int j = 0; j < NUM_GENS; ++j) {
                if (i == j) continue;
                dp_curr[state_idx(i, j, 0)] = (seq_t)1;
            }
        }

        /* store a(2) */
        seq_t sum2 = (seq_t)0;
        for (int i = 0; i < NUM_GENS; ++i)
            for (int j = 0; j < NUM_GENS; ++j)
                if (i != j) sum2 += (seq_t)1;
        cache[2] = sum2;
        max_len   = 2;
    }

    for (idx_t len = max_len + 1; len <= n; ++len) {
        /* zero next array */
        memset(dp_next, 0, STATE_COUNT * sizeof(seq_t));

        /* transitions from all current states */
        for (int prev = 0; prev < NUM_GENS; ++prev) {
            for (int cur = 0; cur < NUM_GENS; ++cur) {
                if (prev == cur) continue;
                size_t base = (prev * NUM_GENS + cur) * KMAX;
                seq_t sum_all = (seq_t)0;

                /* accumulate over all run lengths k */
                for (int k = 0; k < KMAX; ++k) {
                    seq_t val = dp_curr[base + k];
                    if (val) {
                        /* transition by adding 'prev': (prev,cur) -> (cur,prev) */
                        if (k + 1 < KMAX) {
                            size_t idx = ((cur * NUM_GENS + prev) * KMAX + (k + 1));
                            dp_next[idx] += val;
                        }
                    }
                    sum_all += val;
                }

                /* transition by adding any other letter r != cur, r != prev */
                for (int r = 0; r < NUM_GENS; ++r) {
                    if (r == cur || r == prev) continue;
                    size_t idx = ((cur * NUM_GENS + r) * KMAX + 1);
                    dp_next[idx] += sum_all;
                }
            }
        }

        /* compute a(len) as total of dp_next */
        seq_t total = (seq_t)0;
        for (size_t s = 0; s < STATE_COUNT; ++s) total += dp_next[s];

        cache[len] = total;

        /* swap dp arrays for next iteration */
        seq_t *tmp = dp_curr; dp_curr = dp_next; dp_next = tmp;
    }

    max_len = n;
}

/* ---------- exported sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)NUM_GENS;
    expand_to(n);
    return cache[(size_t)n];
}
