/*
 * A129715  Number of runs in all Fibonacci binary words of length n. A
 *          Fibonacci binary word is a binary word having no 00 subword. A run
 *          is a maximal sequence of consecutive identical letters.
 * https://oeis.org/A129715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 39, u64 n <= 83, u128 n <= 174,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Oreise A129715 – Number of runs in all Fibonacci binary words of length n.
   Recurrence: a(0)=0, a(1)=2, a(2)=5; for n≥3
            a(n) = a(n‑1) + a(n‑2) + 2·F(n),
   where F(0)=0, F(1)=1 and F(n)=F(n‑1)+F(n‑2). */

#include <stdlib.h>

/* cache for computed a(n) values – expands only upwards */
static seq_t *a_cache      = NULL;
static idx_t  cache_length = 0;   /* number of stored elements */

/* state needed for incremental Fibonacci computation */
static seq_t fib_prev  = 0;  /* F(0)  */
static seq_t fib_cur   = 1;  /* F(1)  */
static idx_t  fib_index = 1; /* fib_cur == F(fib_index) */

/* make sure a_cache contains values 0 … n inclusive */
static void
ensure_cache(idx_t n)
{
    if (n + 1 <= cache_length)
        return;                 /* already cached */

    /* allocate a new, larger array */
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    if (!new_cache)
        abort();        /* OOM – terminate promptly */

    /* copy any previously cached values */
    if (a_cache) {
        for (idx_t i = 0; i < cache_length; ++i)
            new_cache[i] = a_cache[i];
        free(a_cache);
    }

    /* compute missing entries from cache_length up to n */
    for (idx_t i = cache_length; i <= n; ++i) {
        if (i == 0) {
            new_cache[0] = 0;
        } else if (i == 1) {
            new_cache[1] = 2;
        } else if (i == 2) {
            /* 2·F(2) = 2·1 = 2, but a(2) is given as 5 */
            new_cache[2] = 5;
        } else {
            /* advance Fibonacci to index i */
            while (fib_index < i) {
                seq_t next = fib_prev + fib_cur;
                fib_prev   = fib_cur;
                fib_cur    = next;
                ++fib_index;
            }
            /* fib_cur now == F(i) */
            new_cache[i] = new_cache[i - 1] + new_cache[i - 2] + (fib_cur + fib_cur);
        }
    }

    a_cache      = new_cache;
    cache_length = n + 1;
}

seq_t
a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
