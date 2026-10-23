/*
 * A192799  Coefficient of x in the reduction of the n-th Fibonacci polynomial
 *          by x^3->x^2+2.
 * https://oeis.org/A192799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192799: coefficient of x in the reduction of the n-th Fibonacci polynomial
 * Recurrence (n >= 7):
 *   a(n) = a(n-1) + 3*a(n-2) - 3*a(n-4) + a(n-5) + a(n-6)
 * Initial values:
 *   a(1)=0, a(2)=1, a(3)=0, a(4)=2, a(5)=2, a(6)=5
 */
#include <stdlib.h>

static seq_t *cache = NULL;   /* array holding computed values */
static idx_t cache_cap = 0;   /* current capacity (number of elements allocated) */
static idx_t max_comp  = 0;   /* largest index for which a value has been computed */

static void init_base(idx_t needed)
{
    /* Ensure capacity covers at least indexes 1..6 */
    idx_t cap = needed > 6 ? needed + 1 : 7;   /* +1 for 0‑based indexing */
    if (cache) free(cache);
    cache = (seq_t*)calloc((size_t)cap, sizeof(seq_t));
    if (!cache) exit(1);                     /* abort on allocation failure */
    /* Base cases */
    cache[1] = 0; /* a(1) */
    cache[2] = 1; /* a(2) */
    cache[3] = 0; /* a(3) */
    cache[4] = 2; /* a(4) */
    cache[5] = 2; /* a(5) */
    cache[6] = 5; /* a(6) */
    cache_cap = cap;
    max_comp  = 6;
}

static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_cap) return;
    idx_t new_cap = (n + 1 > cache_cap * 2) ? n + 1 : cache_cap * 2;
    if (new_cap == 0) new_cap = 8;
    seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                         /* abort on allocation failure */
    /* initialise new slots to zero (not strictly required for arbitrary‑precision types) */
    for (idx_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute values up to n (inclusive) using the recurrence */
static void compute_until(idx_t n)
{
    for (idx_t i = max_comp + 1; i <= n; ++i)
    {
        /* All indices used are < i, hence already computed */
        seq_t v1 = cache[i-1];
        seq_t v2 = cache[i-2];
        seq_t v4 = cache[i-4];
        seq_t v5 = cache[i-5];
        seq_t v6 = cache[i-6];
        seq_t val = v1 + 3 * v2 + v5 + v6; /* intermediate sum */
        val -= 3 * v4;                     /* subtract 3*a(i-4) */
        cache[i] = val;
    }
    max_comp = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n<1, return 0 */

    if (!cache)                          /* first call */
        init_base(n);

    if (n > max_comp) {
        if (n + 1 > cache_cap)           /* need more space */
            ensure_capacity(n);
        compute_until(n);
    }
    return cache[n];
}
