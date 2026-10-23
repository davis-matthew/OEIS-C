/*
 * A188476  Diagonal sums of number triangle A188474.
 * https://oeis.org/A188476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows the 8‑term linear recurrence (n+3)*a(n)
 *            +(-4*n-9)*a(n-1) + n*a(n-2) -3*a(n-3) +3*(5*n+1)*a(n-4)
 *            +6*n*a(n-5) +6*(n-1)*a(n-6) +4*(-n+3)*a(n-7)=0. Initial terms:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=12, a(5)=27, a(6)=65,
 *            a(7)=150
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[8] = { (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)6,
                                    (seq_t)12, (seq_t)27, (seq_t)65, (seq_t)150 };

static seq_t *cached = NULL;   /* cache of computed terms a(0)… */
static size_t cache_size = 0;  /* number of elements actually stored  */
static size_t cache_cap  = 0;  /* allocated capacity                 */

/* Grow the cache to hold terms up to index n, computing new values on demand. */
static void ensure_cache(idx_t n)
{
    /* Initialise cache with first 8 terms if necessary. */
    if (cache_size == 0) {
        cache_cap = 8;
        if ((size_t)n + 1 > cache_cap)
            cache_cap = (size_t)n + 1;
        cached = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 8; ++i)
            cached[i] = init_vals[i];
        cache_size = 8;
    }

    /* Resize backing array if needed. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if ((size_t)n + 1 > new_cap)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cached, new_cap * sizeof(seq_t));
        cached = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms sequentially. */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        seq_t a1 = cached[i-1];
        seq_t a2 = cached[i-2];
        seq_t a3 = cached[i-3];
        seq_t a4 = cached[i-4];
        seq_t a5 = cached[i-5];
        seq_t a6 = cached[i-6];
        seq_t a7 = cached[i-7];

        /* Apply the recurrence (as derived from the OEIS formula). */
        seq_t num = (seq_t)(4*i+9) * a1;
        num -= (seq_t)i * a2;
        num += (seq_t)3 * a3;
        num -= (seq_t)3 * (seq_t)(5*i+1) * a4;
        num -= (seq_t)6 * (seq_t)i * a5;
        num -= (seq_t)6 * (seq_t)(i-1) * a6;
        num += (seq_t)(4*i-12) * a7;      /* the a7 term has a + sign */

        seq_t denom = (seq_t)(i+3);
        cached[i] = num / denom;          /* exact division guaranteed */
    }

    cache_size = (size_t)n + 1;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n < 8)
        return init_vals[n];              /* use known initial values */

    ensure_cache(n);
    return cached[n];
}
