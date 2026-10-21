/*
 * A232333  Number of n X 6 0..2 arrays with every 0 next to a 1 and every 1
 *          next to a 2 horizontally or antidiagonally, with no adjacent
 *          elements equal.
 * https://oeis.org/A232333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n‑1) - 60*a(n‑2) - 47*a(n‑3) + 92*a(n‑4) * +
 *            448*a(n‑5) + 448*a(n‑6) + 256*a(n‑7) for n > 9 * Initial values:
 *            a(1)=16, a(2)=162, a(3)=1470, a(4)=13170, * a(5)=117690,
 *            a(6)=1047700, a(7)=9298730, * a(8)=82332898, a(9)=727588212
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[10] = {
    0,                      /* unused; indices start at 1 */
    (seq_t)16,              /* a(1) */
    (seq_t)162,             /* a(2) */
    (seq_t)1470,            /* a(3) */
    (seq_t)13170,           /* a(4) */
    (seq_t)117690,          /* a(5) */
    (seq_t)1047700,         /* a(6) */
    (seq_t)9298730,         /* a(7) */
    (seq_t)82332898,        /* a(8) */
    (seq_t)727588212        /* a(9) */
};

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    static seq_t *cache = NULL;
    static idx_t max_computed = 0;
    static idx_t capacity   = 0;

    /* initialise cache on first call */
    if (!cache) {
        capacity = n + 1;
        if (capacity < 16) capacity = 16;          /* ensure space for init */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;   /* allocation failure */

        /* zero or leave unused 0 entry */
        cache[0] = (seq_t)0;
        for (idx_t i = 1; i <= 9 && i < capacity; ++i)
            cache[i] = init_vals[i];
        if (n < 9)
            max_computed = n;
        else
            max_computed = 9;
    }

    /* grow cache if necessary */
    if (n + 1 > capacity) {
        capacity = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;   /* reallocation failure */
        /* zero new entries */
        for (idx_t i = max_computed + 1; i < capacity; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
    }

    /* compute missing terms */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t term;
        if (i <= 9) {
            term = init_vals[i];
        } else {
            /* compute according to the recurrence, all unsigned arithmetic */
            term = 16 * cache[i - 1];
            term += 92 * cache[i - 4];
            term += 448 * cache[i - 5];
            term += 448 * cache[i - 6];
            term += 256 * cache[i - 7];
            term -= 60 * cache[i - 2];
            term -= 47 * cache[i - 3];
        }
        cache[i] = term;
    }

    max_computed = n;
    return cache[n];
}
