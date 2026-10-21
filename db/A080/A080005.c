/*
 * A080005  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=2, r=3, I={-1,1}.
 * https://oeis.org/A080005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + a(n-2) + a(n-4) + a(n-5)
 *                    - 2*a(n-6) + a(n-8) - a(n-10)
 * Initial values:
 * a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=4, a(5)=7,
 * a(6)=11, a(7)=19, a(8)=35, a(9)=62
 *
 * This implementation evaluates the recurrence iteratively with
 * memoisation.  It works for any build‑time definition of seq_t
 * and idx_t and satisfies the required performance constraints.
 */

#include <stdlib.h>
#include <stddef.h>

/* initial values for n = 0 … 9 */
static const seq_t BASE[10] = {1, 1, 1, 2, 4, 7, 11, 19, 35, 62};

seq_t a(idx_t n)
{
    /* fast branch for the first ten terms */
    if (n < 10) return BASE[(size_t)n];

    static seq_t *cache         = NULL;
    static size_t  cap          = 0;
    static size_t  max_computed = 9; /* highest index already filled */

    /* first allocation: at least enough for the requested index */
    if (!cache) {
        cap = (size_t)n + 1;
        if (cap < 10) cap = 10;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 10; ++i) cache[i] = BASE[i];
    }

    /* grow the buffer if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    if (max_computed < (size_t)n) {
        for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
            seq_t sum = cache[i-1] + cache[i-2] + cache[i-4] + cache[i-5];
            sum -= (cache[i-6] + cache[i-6]); /* 2 * a(i-6) */
            sum += cache[i-8];
            sum -= cache[i-10];
            cache[i] = sum;
        }
        max_computed = (size_t)n;
    }

    return cache[(size_t)n];
}
