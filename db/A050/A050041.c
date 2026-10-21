/*
 * A050041  a(n) = a(n-1) + a(m) for n >= 4, where m = 2^(p+1) + 2 - n and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = 1, a(2) = 2, and a(3) = 1.
 * https://oeis.org/A050041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=1; For n>=4: a(n) = a(n-1) + a(m), where m
 *            = 2^(p+1) + 2 - n and p is the unique integer with 2^p < n-1 <=
 *            2^(p+1)
 * Range:     u8 n <= 21, u16 n <= 77, u32 n <= 518, u64 n <= 4480,
 *            u128 n <= 8193, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for memoization. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;      /* number of allocated slots (>=1)   */
static idx_t computed_upto = 0;   /* highest n that has been computed */

/* Ensure that the cache can hold indices up to 'need' (inclusive). */
static void ensure_cache_cap(idx_t need)
{
    size_t cap = (size_t)need + 1;       /* indices start at 1 */
    if (cap <= cache_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!tmp)
        abort();                         /* unrecoverable allocation failure */
    cache = tmp;
    cache_cap = cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n<1, safe guard */

    if (computed_upto < n) {
        ensure_cache_cap(n);

        for (idx_t i = computed_upto + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)1;
            } else if (i == 2) {
                cache[i] = (seq_t)2;
            } else if (i == 3) {
                cache[i] = (seq_t)1;
            } else {
                /* Find p such that 2^p < i-1 <= 2^(p+1). */
                idx_t p = 0;
                while (((idx_t)1 << (p + 1)) < (i - 1))
                    ++p;

                idx_t powi = ((idx_t)1 << (p + 1));
                idx_t m = powi + 2 - i;      /* 1 <= m < i for i>=4 */
                cache[i] = cache[i - 1] + cache[m];
            }
        }
        computed_upto = n;
    }

    return cache[n];
}
