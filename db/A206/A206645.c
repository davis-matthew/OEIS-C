/*
 * A206645  Number of nX3 0..2 arrays with no occurrence of three equal
 *          elements in a row horizontally or vertically, and new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A206645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A206645
   Recurrence: a(n) = 15*a(n-1) + 66*a(n-2) - 145*a(n-3) + 13*a(n-4)
                - 64*a(n-5) + 156*a(n-6) - 36*a(n-7) - 8*a(n-9) - 8*a(n-10)
   Initial values: a(1)=4, a(2)=96, a(3)=1625, a(4)=30145, a(5)=545520,
                   a(6)=9937823, a(7)=180716227, a(8)=3287842038,
                   a(9)=59809329737, a(10)=1088032590151
*/

#include <stdlib.h>

static const seq_t base_vals[10] = {
    (seq_t)4ULL,
    (seq_t)96ULL,
    (seq_t)1625ULL,
    (seq_t)30145ULL,
    (seq_t)545520ULL,
    (seq_t)9937823ULL,
    (seq_t)180716227ULL,
    (seq_t)3287842038ULL,
    (seq_t)59809329737ULL,
    (seq_t)1088032590151ULL
};

/* cache[0] holds a(1), cache[1] holds a(2), ... */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;   /* number of elements allocated */
static idx_t  computed_max = 0; /* largest n for which a(n) is computed */

/* ensure cache can hold at least n elements, reallocating if necessary */
static void ensure_cache_size(idx_t n)
{
    if (cache_size < n)
    {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failed – terminate */
        cache = tmp;
        cache_size = n;
    }
}

/* initialise the first ten terms if they are not already present */
static void init_base_terms(void)
{
    if (computed_max < 10)
    {
        if (cache_size < 10) {
            /* grow the cache to at least 10 elements */
            seq_t *tmp = (seq_t *)realloc(cache, 10 * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_size = 10;
        }
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = base_vals[i];
        computed_max = 10;
    }
}

seq_t a(idx_t n)
{
    if (n < 1u) return 0;

    ensure_cache_size(n);
    init_base_terms();

    if (n <= computed_max)
        return cache[n - 1];

    /* compute a(k) for k from computed_max+1 up to n */
    for (idx_t k = computed_max + 1; k <= n; ++k)
    {
        /* indices in cache are zero-based: a(k-1) -> cache[k-2], etc. */
        seq_t pos = (seq_t)15 * cache[k - 2] +   /* a(k-1) */
                    (seq_t)66 * cache[k - 3] +   /* a(k-2) */
                    (seq_t)13 * cache[k - 5] +   /* a(k-4) */
                    (seq_t)156 * cache[k - 7];   /* a(k-6) */

        seq_t neg = (seq_t)145 * cache[k - 4] +   /* a(k-3) */
                    (seq_t)64 * cache[k - 6] +   /* a(k-5) */
                    (seq_t)36 * cache[k - 8] +   /* a(k-7) */
                    (seq_t)8  * cache[k -10] +   /* a(k-9) */
                    (seq_t)8  * cache[k -11];    /* a(k-10) */

        cache[k - 1] = pos - neg;   /* final result */
    }

    computed_max = n;
    return cache[n - 1];
}
