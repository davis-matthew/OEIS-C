/*
 * A073968  Number of strings of length n over Z_5 with trace 1 and subtrace
 *          2.
 * https://oeis.org/A073968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A073968:
    Number of strings of length n over Z_5 with trace 1 and subtrace 2.
    Recurrence:
        a(n; t, s) = Σ_{v=0}^{4} a(n-1; (t-v) mod5, (s - v*(t-v)) mod5)
    Initial condition: a(0;0,0)=1, all other a(0; t,s)=0.
    The desired value a(n) is a(n;1,2).  */
#include <stdlib.h>

extern seq_t a(idx_t n) __attribute__((visibility("default")));

static seq_t *cache = NULL;        /* array of already computed a(n) values   */
static size_t cache_capacity = 0;  /* number of allocated entries (size)     */
static idx_t last_computed = 0;    /* largest n for which a(n) has been computed */

/* DP states for a single length: indexed by t*5+s (t,s in 0..4) */
static seq_t dp_prev[25];
static seq_t dp_cur[25];
static int initialized = 0;

static void ensure_capacity(idx_t n)
{
    /* need at least n+1 slots (ignore slot 0) */
    size_t needed = (size_t)n + 1;
    if (needed <= cache_capacity) return;

    /* allocate a larger buffer */
    size_t new_cap = needed;
#if defined(__cplusplus)
    seq_t *tmp = new seq_t[new_cap];          /* construct elements */
    if (cache) {
        for (size_t i = 0; i < cache_capacity; ++i) tmp[i] = cache[i];
        delete [] cache;                     /* destroy old array */
    }
    cache = tmp;
#else
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && cache) free(cache);          /* out of memory: fall back to NULL */
    cache = tmp;
#endif
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;    /* below offset, undefined – return 0 */

    /* initialise DP tables once */
    if (!initialized) {
        for (int i = 0; i < 25; ++i) dp_prev[i] = 0;
        dp_prev[0] = 1;                /* a(0;0,0) = 1 */
        initialized = 1;
    }

    /* avoid recomputation if already known */
    if (n <= last_computed) return cache[n];

    /* make sure the cache can hold all needed terms */
    ensure_capacity(n);

    /* compute all lengths up to n */
    for (idx_t cur = last_computed + 1; cur <= n; ++cur) {
        /* clear current DP */
        for (int i = 0; i < 25; ++i) dp_cur[i] = 0;

        /* add one more symbol v to all previous strings */
        for (idx_t t = 0; t < 5; ++t) {
            for (idx_t s = 0; s < 5; ++s) {
                seq_t val = dp_prev[t * 5 + s];
                if (!val) continue;
                for (idx_t v = 0; v < 5; ++v) {
                    idx_t nt = t + v;
                    if (nt >= 5) nt -= 5;
                    idx_t ns = s + v * t;
                    ns %= 5;
                    dp_cur[nt * 5 + ns] += val;
                }
            }
        }

        /* dp_cur now holds counts for length 'cur' */
        for (int i = 0; i < 25; ++i) dp_prev[i] = dp_cur[i];

        /* store the desired value a(cur) = a(cur;1,2) */
        cache[cur] = dp_prev[1 * 5 + 2];
    }

    last_computed = n;
    return cache[n];
}
