/*
 * A179574  Number of permutations of 1..n+7 with the number moved left
 *          exceeding the number moved right by n.
 * https://oeis.org/A179574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A062866(n+7,n)
 * Range:     u8 none, u16 n <= 2, u32 n <= 11, u64 n <= 31, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (12‑term) with coefficients:
   a(n)=20a(n-1)-180a(n-2)+964a(n-3)-3422a(n-4)+8484a(n-5)
        -15068a(n-6)+19324a(n-7)-17769a(n-8)+11432a(n-9)
        -4888a(n-10)+1248a(n-11)-144a(n-12)
   Initial values (n=1..12) are computed from the definition.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms.  */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* Initial 12 terms (a(1)..a(12)).  Store as const to avoid
       re‑computing and to keep the function purely algorithmic.  */
    static const seq_t init[12] = {
        7736ULL,        34659ULL,      138022ULL, 511412ULL,
        1807840ULL,     6188910ULL,     20713252ULL,68199003ULL,
        221845712ULL,   715077749ULL,  2288733954ULL,7285025030ULL
    };

    if (n == 0)
        return 0;                      /* undefined for n<1, return 0 */

    /* If the result already cached, return immediately.  */
    if (n <= cache_len)
        return cache[n - 1];

    /* Ensure enough storage for the requested index.  */
    if ((size_t)n > cache_cap) {
        size_t newcap = cache_cap ? cache_cap * 2 : 12;
        if ((size_t)n > newcap)
            newcap = (size_t)n;
        seq_t *newptr = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!newptr)
            return 0;                  /* memory allocation failed, fail silently */
        /* Copy existing terms.  */
        for (size_t i = 0; i < cache_len; ++i)
            newptr[i] = cache[i];
        /* Free old buffer if present.  */
        if (cache)
            free(cache);
        cache   = newptr;
        cache_cap = newcap;
    }

    /* If we have not yet stored the first 12 values, copy them.  */
    if (cache_len < 12) {
        size_t to_copy = 12 - cache_len;
        for (size_t i = 0; i < to_copy; ++i)
            cache[cache_len + i] = init[cache_len + i];
        cache_len = 12;
    }

    /* Compute terms from cache_len+1 up to n inclusive.  */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        /* Positive part of the recurrence.  */
        seq_t pos = 20 * cache[k - 1 - 1];          /* a(k-1)   */
        pos += 964 * cache[k - 3 - 1];              /* a(k-3)   */
        pos += 8484 * cache[k - 5 - 1];             /* a(k-5)   */
        pos += 19324 * cache[k - 7 - 1];            /* a(k-7)   */
        pos += 11432 * cache[k - 9 - 1];            /* a(k-9)   */
        pos += 1248  * cache[k - 11 - 1];           /* a(k-11)  */

        /* Negative part of the recurrence.  */
        seq_t neg = 180  * cache[k - 2 - 1];        /* a(k-2)   */
        neg += 3422 * cache[k - 4 - 1];            /* a(k-4)   */
        neg += 15068 * cache[k - 6 - 1];           /* a(k-6)   */
        neg += 17769 * cache[k - 8 - 1];           /* a(k-8)   */
        neg += 4888  * cache[k - 10 - 1];          /* a(k-10)  */
        neg += 144   * cache[k - 12 - 1];          /* a(k-12)  */

        /* The recurrence guarantees that pos >= neg.  */
        cache[k - 1] = pos - neg;
    }

    cache_len = n;
    return cache[n - 1];
}
