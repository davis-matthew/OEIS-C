/*
 * A232291  Number of nX4 0..2 arrays with every 0 next to a 1 and every 1
 *          next to a 2 horizontally or antidiagonally.
 * https://oeis.org/A232291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A232291 – Number of n×4 0..2 arrays with every 0 next to a 1 and every 1 next to a 2 horizontally or antidiagonally
    Empirical recurrence (OEIS):
        a(n) = 30*a(n-1)+171*a(n-2)+93*a(n-3)-496*a(n-4)-281*a(n-5)+186*a(n-6)+206*a(n-7)-173*a(n-8)+45*a(n-9)-4*a(n-10)
    Base values (1‑based, given in the OEIS entry):
        a(1)=15, a(2)=587, a(3)=20563, a(4)=718483, a(5)=25116353,
        a(6)=877968487, a(7)=30690409685, a(8)=1072818688305,
        a(9)=37501616113029, a(10)=1310912294899905
    The implementation memoises the sequence so that calls in any order
    are efficient.
*/
#include <stdbool.h>
#include <stdlib.h>

/* Static cache of computed values.  cache[0] is unused to keep indices 1‑based. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;        /* current allocated length (number of slots)   */
static idx_t max_computed = 0;     /* highest index that has a valid value         */
static bool base_inited = false;   /* true once the first ten terms are entered    */

/* Initialise the first ten terms once.  Called before any computation. */
static void init_base(void)
{
    if (base_inited) return;
    /* Ensure at least 11 slots (index 0 unused). */
    if (cache_len < 11) {
        cache_len = 11;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE); /* fatal if allocation fails */
    }
    cache[1]  = (seq_t)15;
    cache[2]  = (seq_t)587;
    cache[3]  = (seq_t)20563;
    cache[4]  = (seq_t)718483;
    cache[5]  = (seq_t)25116353;
    cache[6]  = (seq_t)877968487;
    cache[7]  = (seq_t)30690409685ull;
    cache[8]  = (seq_t)1072818688305ull;
    cache[9]  = (seq_t)37501616113029ull;
    cache[10] = (seq_t)1310912294899905ull;
    max_computed = 10;
    base_inited = true;
}

/* Grow the cache so that indices up to n are valid slots. */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;           /* already large enough */
    /* Grow to at least n+1 slots, at least double the current size. */
    idx_t new_len = cache_len * 2;
    if (new_len <= n) new_len = n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);
    cache_len = new_len;
}

/* Compute all missing terms up to n (inclusive).  Must be called after init_base. */
static void compute_up_to(idx_t n)
{
    init_base();
    if (n <= max_computed) return;       /* nothing to do */
    ensure_cache(n);
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t v =   (seq_t)30   * cache[i-1]
                  + (seq_t)171  * cache[i-2]
                  + (seq_t)93   * cache[i-3]
                  - (seq_t)496  * cache[i-4]
                  - (seq_t)281  * cache[i-5]
                  + (seq_t)186  * cache[i-6]
                  + (seq_t)206  * cache[i-7]
                  - (seq_t)173  * cache[i-8]
                  + (seq_t)45   * cache[i-9]
                  - (seq_t)4    * cache[i-10];
        cache[i] = v;
    }
    max_computed = n;
}

/* External entry point required by the grading system. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* sequence is 1‑based */
    compute_up_to(n);
    return cache[n];
}
