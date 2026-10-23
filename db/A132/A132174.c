/*
 * A132174  Index of starting position of n-th generation of terms in A063882.
 * https://oeis.org/A132174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + a(n-5) - 3*a(n-6) + 2*a(n-7) * for
 *            n > 8, with initial values: * a(1)=1, a(2)=5, a(3)=10, a(4)=21,
 *            a(5)=44, a(6)=92, * a(7)=189, a(8)=385
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* 0‑based array: cache[i] = a(i+1) */
static idx_t cacheSize = 0;      /* number of entries currently stored */

/* initialise the first eight values */
static void init_cache(void)
{
    if (cacheSize >= 8)
        return;
    if (!cache)
        cache = (seq_t *)malloc(8 * sizeof(seq_t));
    static const seq_t init[8] = {1, 5, 10, 21, 44, 92, 189, 385};
    for (idx_t i = 0; i < 8; ++i)
        cache[i] = init[i];
    cacheSize = 8;
}

/* extend cache up to (but not including) target n (1‑based). */
static void ensure_up_to(idx_t n)
{
    if (cacheSize >= n)
        return;
    if (!cache)
        cache = (seq_t *)malloc(n * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!cache)
        abort(); /* memory failure */

    init_cache();                 /* guarantees cache[0..7] set */

    /* compute successive terms using the recurrence */
    for (idx_t i = cacheSize; i < n; ++i) {
        /* i is 0‑based index, so a(i+1) depends on cache[i-1], etc. */
        seq_t t1 = 3 * cache[i - 1];  /* 3*a(i)  */
        t1 += cache[i - 5];          /* + a(i-4) */
        t1 += 2 * cache[i - 7];      /* + 2*a(i-6) */

        seq_t t2 = 2 * cache[i - 2];  /* 2*a(i-1) */
        t2 += 3 * cache[i - 6];      /* + 3*a(i-5) */

        cache[i] = t1 - t2;          /* final subtraction */
    }
    cacheSize = n;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; behaviour below offset is undefined */
        return 0;
    if (n <= 8) {
        static const seq_t init[8] = {1, 5, 10, 21, 44, 92, 189, 385};
        return init[n - 1];
    }
    ensure_up_to(n);
    return cache[n - 1];
}
