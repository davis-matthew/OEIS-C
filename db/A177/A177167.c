/*
 * A177167  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=10, k=0 and l=-1.
 * https://oeis.org/A177167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A177167
 *   a(0)=1, a(1)=10
 *   a(n+1)=∑_{p=0}^{n} a(p)*a(n-p) - 1   (n ≥ 0)
 *   Thus for n ≥ 2 : a(n)=∑_{p=0}^{n-1} a(p)*a(n-1-p) - 1
 */

#include <stddef.h>
#include <stdlib.h>

/* memoisation cache */
static seq_t *cache = NULL;       /* dynamic array of seq_t */
static size_t cap  = 0;           /* current capacity of cache */
static idx_t  up_to = 0;          /* highest index already computed */

/* initialise the cache with a(0) and a(1) */
static void init_cache(void)
{
    if (cap == 0) {
        cap = 4;                     /* room for a few initial terms */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        up_to = 1;
    }
}

/* ensure that cache contains values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    init_cache();                     /* makes sure we have a cache */

    if (n <= up_to)                  /* nothing to do */
        return;

    /* grow the array if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* compute new terms */
    for (idx_t i = up_to + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* a(i) = ∑_{p=0}^{i-1} a(p) * a(i-1-p) - 1 */
        for (idx_t p = 0; p < i; ++p) {
            sum += cache[p] * cache[i - 1 - p];
        }
        cache[i] = sum - (seq_t)1;
    }
    up_to = n;
}

/* public function required by the exercise */
seq_t a(idx_t n)
{
    /* base cases can be returned immediately */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;

    ensure_cache(n);
    return cache[n];
}
