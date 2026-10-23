/*
 * A152518  1/36 of the number of permutations of 8 indistinguishable copies
 *          of 1..n with exactly 2 local maxima.
 * https://oeis.org/A152518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 183*a(n-1) - 3051*a(n-2) + 13365*a(n-3) (n>3) initial
 *            values: a(1)=0, a(2)=7, a(3)=1290
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms, indexed from 1 */
static idx_t  computed_max = 0;      /* highest n for which cache[n] is valid */

/* Ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    } else if (computed_max < n) {
        cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    }
}

/* Compute terms up to n using the linear recurrence */
static void compute_until(idx_t n)
{
    if (computed_max >= n) return;

    ensure_cache(n);
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)0;
        } else if (i == 2) {
            cache[i] = (seq_t)7;
        } else if (i == 3) {
            cache[i] = (seq_t)1290;
        } else {
            seq_t t1 = (seq_t)183 * cache[i - 1];
            seq_t t2 = (seq_t)3051 * cache[i - 2];
            seq_t t3 = (seq_t)13365 * cache[i - 3];
            cache[i] = t1 - t2 + t3;
        }
    }
    computed_max = n;
}

/* Public function returning a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* offset handling – undefined behaviour is fine */
    compute_until(n);
    return cache[n];
}
