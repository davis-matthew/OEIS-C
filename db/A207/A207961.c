/*
 * A207961  Number of 3 X n 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence:  a(1)=6, a(2)=36, a(3)=102, a(4)=281, a(5)=699, a(6)=1799,
 * a(7)=4706, a(8)=12161, a(9)=31356, a(10)=81206, a(11)=210185,
 * a(12)=543399, a(13)=1405547;
 * For n > 13:
 * a(n)=a(n-1)+a(n-2)+7*a(n-3)+4*a(n-4)-7*a(n-6)-6*a(n-7)+6*a(n-9)+
 *      2*a(n-10)-a(n-11)-a(n-12).
 *
 * This function returns the n-th term of OEIS A207961 (1‑based index).
 */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* not defined for n < 1 */

    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cap  = 0;   /* allocated capacity */
    static size_t size = 0;   /* next free index  (= max computed + 1) */
    static bool   init_done = false;

    /* Initial values for n = 1..13 */
    static const seq_t init_values[14] = {
        0ULL, /* dummy for index 0 */
        (seq_t)6,      (seq_t)36,      (seq_t)102,    (seq_t)281,
        (seq_t)699,    (seq_t)1799,    (seq_t)4706,   (seq_t)12161,
        (seq_t)31356,  (seq_t)81206,   (seq_t)210185, (seq_t)543399,
        (seq_t)1405547
    };

    /* One‑time initialization of cache and base terms */
    if (!init_done) {
        size_t need = 14;          /* indices 0..13 inclusive */
        cache = (seq_t*)malloc(need * sizeof(seq_t));
        if (!cache) abort();      /* out of memory */
        for (size_t i = 0; i < need; ++i)
            cache[i] = init_values[i];
        cap  = need;
        size = need;              /* next free = 14 */
        init_done = true;
    }

    /* Resize cache if we need more space */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();        /* out of memory */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute terms up to n */
    for (size_t i = size; i <= (size_t)n; ++i) {
        /* i <= 13: already filled by init_values */
        if (i <= 13) {
            /* nothing to do */
        } else {
            /* recurrence formula for i > 13 */
            // Using 1‑based indices: cache[0] unused
            seq_t val = cache[i-1]
                       + cache[i-2]
                       + 7 * cache[i-3]
                       + 4 * cache[i-4]
                       - 7 * cache[i-6]
                       - 6 * cache[i-7]
                       + 6 * cache[i-9]
                       + 2 * cache[i-10]
                       -     cache[i-11]
                       -     cache[i-12];
            cache[i] = val;
        }
    }
    size = (size_t)n + 1;     /* now we have computed up to n */

    return cache[n];
}
