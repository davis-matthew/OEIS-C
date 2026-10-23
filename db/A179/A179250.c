/*
 * A179250  Numbers that have 10 terms in their Zeckendorf representation.
 * https://oeis.org/A179250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes nth integer whose Zeckendorf representation uses exactly
   10 Fibonacci terms.  Uses a simple search with a cache of all
   previously found values. */

#include <stdlib.h>
#include <stdbool.h>

/* Fibonacci numbers (Zeckendorf basis): 1,2,3,5,8,…  */
static seq_t *fibs = NULL;
static size_t fib_cnt = 0;

/* Build Fibonacci array once.  90 terms are enough to cover the
   values needed for n up to 10 000 and also fit in 64‑bit. */
static void init_fibs(void)
{
    if (fib_cnt) return;
    fib_cnt = 90;
    fibs = (seq_t*)malloc(fib_cnt * sizeof(seq_t));
    if (!fibs) abort();
    fibs[0] = (seq_t)1;
    fibs[1] = (seq_t)2;
    for (size_t i = 2; i < fib_cnt; ++i)
        fibs[i] = fibs[i-1] + fibs[i-2];
}

/* Greedy Zeckendorf decomposition: return the number of terms
   required to represent n. */
static int count_terms(seq_t n)
{
    int cnt = 0;
    while (n > (seq_t)0) {
        size_t i = fib_cnt;                     /* start beyond last index */
        while (i > 0 && fibs[--i] > n)           /* find largest fib <= n */
            ;
        n -= fibs[i];
        ++cnt;
    }
    return cnt;
}

/* Cache of values with exactly 10 terms.  cache stores results in
   increasing order of n. */
static seq_t *cache  = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static idx_t next_candidate = 1;               /* next integer to examine */

/* Ensure cache contains at least n elements.  Expand dynamically
   as needed. */
static void ensure_cache(idx_t n)
{
    if (cache_size >= (size_t)n) return;

    while (cache_size < (size_t)n) {
        seq_t val = (seq_t)next_candidate;
        if (count_terms(val) == 10) {
            if (cache_size >= cache_cap) {       /* grow cache */
                size_t new_cap = (cache_cap == 0 ? 16 : cache_cap << 1);
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = val;
        }
        ++next_candidate;
    }
}

/* Public sequence routine, 1‑based index. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_fibs();
        initialized = true;
    }
    if (n == 0) return (seq_t)0;                /* unused, harmless */
    ensure_cache(n);
    return cache[n-1];
}
