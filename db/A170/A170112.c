/*
 * A170112  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>=1
   a(n) = sum over all words of length n that avoid
   adjacent equal generators and avoid any alternating
   subsequence of length 38 or more.
   The recurrence is implemented by dynamic programming
   over word states: (prev, last, len), where
   prev and last are the last two generators
   and len is the current length of an alternating
   run (max 37). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* number of generators */
#define GEN 7
/* maximal allowed alternating run length (38 would be reducible) */
#define MAX_LEN 37
/* total number of distinct states: 7 same‑letter states + 42*36 distinct ones */
#define STATE_COUNT (7 + 42*36)

/* state mapping tables */
static int state_index[GEN][GEN][MAX_LEN + 1];
static int prev_arr[STATE_COUNT];
static int last_arr[STATE_COUNT];
static int len_arr[STATE_COUNT];
static int init_done = 0;

/* caches for computed terms and DP arrays */
static seq_t *a_cache      = NULL;
static idx_t  a_cache_size = 0;            /* number of entries currently allocated */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;
static idx_t  dp_cur_len = 0;              /* maximum n already computed */

/* ------------------------------------------------------------------ */
/* initialise state tables once                                          */
static void init_states(void)
{
    int i, j, l, idx = 0;

    /* set all to -1 */
    for (i = 0; i < GEN; ++i)
        for (j = 0; j < GEN; ++j)
            for (l = 0; l <= MAX_LEN; ++l)
                state_index[i][j][l] = -1;

    /* same‑letter states, run length = 1 */
    for (i = 0; i < GEN; ++i) {
        state_index[i][i][1] = idx;
        prev_arr[idx] = i;
        last_arr[idx] = i;
        len_arr[idx]  = 1;
        ++idx;
    }

    /* distinct‑letter states, alternating runs of length 2..MAX_LEN */
    for (i = 0; i < GEN; ++i)
        for (j = 0; j < GEN; ++j)
            if (i != j)
                for (l = 2; l <= MAX_LEN; ++l) {
                    state_index[i][j][l] = idx;
                    prev_arr[idx] = i;
                    last_arr[idx] = j;
                    len_arr[idx]  = l;
                    ++idx;
                }
    /* idx should now equal STATE_COUNT */
    (void)idx; /* silence unused variable warning if compiled as C */
}

/* ------------------------------------------------------------------ */
/* ensure the cache a_cache can hold values up to 'need'               */
static void ensure_cache(idx_t need)
{
    if (need + 1 <= a_cache_size)
        return;

    seq_t *new_cache = (seq_t *)realloc(a_cache, (need + 1) * sizeof(seq_t));
    if (!new_cache)
        exit(1); /* memory failure */

    /* initialise new entries to zero */
    for (idx_t i = a_cache_size; i <= need; ++i)
        new_cache[i] = 0;

    a_cache = new_cache;
    a_cache_size = need + 1;
}

/* ------------------------------------------------------------------ */
/* public function                                                        */
seq_t a(idx_t n)
{
    /* one‑time initialisation */
    if (!init_done) {
        init_states();
        init_done = 1;

        /* cache a(0) */
        ensure_cache(0);
        a_cache[0] = 1;

        /* allocate DP arrays */
        dp_cur  = (seq_t *)malloc(STATE_COUNT * sizeof(seq_t));
        dp_next = (seq_t *)malloc(STATE_COUNT * sizeof(seq_t));
        if (!dp_cur || !dp_next)
            exit(1);
        memset(dp_cur, 0, STATE_COUNT * sizeof(seq_t));
        memset(dp_next, 0, STATE_COUNT * sizeof(seq_t));
    }

    /* extend cache if required */
    if (n >= a_cache_size)
        ensure_cache(n);

    /* return cached value if available */
    if (a_cache[n] != 0)
        return a_cache[n];

    /* compute missing terms up to n */
    while (dp_cur_len < n) {
        /* clear next layer */
        memset(dp_next, 0, STATE_COUNT * sizeof(seq_t));

        if (dp_cur_len == 0) {
            /* initialize for length 1: words of single generator */
            for (int g = 0; g < GEN; ++g) {
                int s = state_index[g][g][1];
                dp_next[s] = 1;
            }
        } else {
            /* transition from current layer to next */
            for (int s = 0; s < STATE_COUNT; ++s) {
                seq_t val = dp_cur[s];
                if (val == 0)
                    continue;
                int prev  = prev_arr[s];
                int last  = last_arr[s];
                int len   = len_arr[s];

                for (int newg = 0; newg < GEN; ++newg) {
                    if (newg == last)
                        continue;          /* adjacent equal forbidden */

                    int ns;
                    if (newg == prev) {
                        if (len == MAX_LEN)
                            continue;      /* would create alternating run of length 38 */
                        ns = state_index[last][newg][len + 1];
                    } else {
                        ns = state_index[last][newg][2];
                    }
                    dp_next[ns] += val;
                }
            }
        }

        /* swap layers */
        seq_t *tmp = dp_cur;
        dp_cur    = dp_next;
        dp_next   = tmp;

        /* advance length counter */
        ++dp_cur_len;

        /* sum all states to get a(dp_cur_len) */
        seq_t total = 0;
        for (int s = 0; s < STATE_COUNT; ++s)
            total += dp_cur[s];

        ensure_cache(dp_cur_len);
        a_cache[dp_cur_len] = total;
    }

    return a_cache[n];
}
