/*
 * A169220  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1, a(n)=13*(a(n-1)+...+a(n-27)) - 91*a(n-28) +
 *            r(n), where r(n)=2 for 1<=n<=27, r(28)=1, and r(n)=0 otherwise
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Cache for memoisation */
static seq_t *cache = NULL;         /* allocated array of the sequence values   */
static idx_t cache_cap = 0;         /* allocated size                         */
static idx_t computed_up_to = 0;    /* highest index whose value is known  */
static bool cache_initialised = false;  /* whether a[0] has been set */

/* Ensure that cache has room for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* allocate */
        size_t sz = (size_t)(n + 1);
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (cache == NULL) abort();
        cache_cap = sz;
    } else if (cache_cap <= n) {
        /* enlarge */
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_sz;
    }
}

/* Compute the sequence value a(n) */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Initialise a[0] once */
    if (!cache_initialised) {
        cache[0] = (seq_t)1;
        computed_up_to = 0;
        cache_initialised = true;
    }

    /* Compute missing entries up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        /* Sum of the last 27 terms (or fewer if i < 27) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= 27 && i >= k; ++k) {
            sum += cache[i - k];
        }

        /* 91 * a(i-28) if i >= 28, otherwise 0 */
        seq_t minus_term = (seq_t)0;
        if (i >= 28) {
            minus_term = (seq_t)91 * cache[i - 28];
        }

        /* The RHS adjustment r(i) */
        seq_t r = (seq_t)0;
        if (i <= 27) {
            r = (seq_t)2;
        } else if (i == 28) {
            r = (seq_t)1;
        }

        /* Recurrence: a(i) = 13*sum - 91*a(i-28) + r(i) */
        cache[i] = (seq_t)13 * sum - minus_term + r;
    }

    computed_up_to = n;
    return cache[n];
}
