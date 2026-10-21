/*
 * A231444  Number of (n+1) X (1+1) 0..3 arrays with no element unequal to a
 *          strict majority of its horizontal and antidiagonal neighbors, with
 *          values 0..3 introduced in row major order.
 * https://oeis.org/A231444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 41*a(n-2) + 65*a(n-3) - 43*a(n-4) + 9*a(n-5)
 *            initial values: (n, a(n)) = (1,2),(2,6),(3,23),(4,102),(5,492)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* cache for memoised terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* multiply a seq_t by an unsigned integer constant */
static inline seq_t mul_const(seq_t x, unsigned m)
{
    return x * m;
}

/* ensure the cache can hold at least upto 'n' terms (1‑based) */
static void ensure_cache(idx_t n)
{
    /* if already big enough */
    if ((size_t)n <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < (size_t)n) new_cap = n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();                /* stop if out of memory */
    cache = new_cache;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, 0 is safe default */

    /* enlarge cache if needed */
    ensure_cache(n);

    /* compute new terms if necessary */
    while (cache_len < (size_t)n) {
        size_t i = cache_len + 1;   /* current 1‑based index to compute */

        if (i <= 5) {   /* initial values */
            static const seq_t init[5] = { 2, 6, 23, 102, 492 };
            cache[i - 1] = init[i - 1];
        } else {
            /* recurrence: i > 5 */
            seq_t t1 = mul_const(cache[i - 2], 11); /* a[i-1] */
            seq_t t2 = mul_const(cache[i - 3], 41); /* a[i-2] */
            seq_t t3 = mul_const(cache[i - 4], 65); /* a[i-3] */
            seq_t t4 = mul_const(cache[i - 5], 43); /* a[i-4] */
            seq_t t5 = mul_const(cache[i - 6], 9);  /* a[i-5] */

            /* sum positive contributions first */
            seq_t sum = t1 + t3 + t5;
            seq_t sub = t2 + t4;
            cache[i - 1] = sum - sub;  /* guaranteed non‑negative */
        }

        cache_len = i;
    }

    return cache[n - 1];
}
