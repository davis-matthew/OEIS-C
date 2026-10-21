/*
 * A202446  Number of (n+2) X 9 binary arrays avoiding patterns 001 and 110 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) -11*a(n-3) +6*a(n-4) +14*a(n-5)
 *            -14*a(n-6) -6*a(n-7) +11*a(n-8) -a(n-9) -3*a(n-10) +a(n-11)
 *            Initial values: a(1) = 888, a(2) = 1568, a(3) = 3652, a(4) =
 *            7238, a(5) = 14338, a(6) = 25576, a(7) = 45156, a(8) = 74150,
 *            a(9) = 120170, a(10) = 184864, a(11) = 280724, a(12) = 41...
 * Range:     u8 none, u16 n <= 7, u32 n <= 71, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cache for computed values */
static size_t cache_cap = 0;          /* current cache capacity (number of slots) */
static idx_t cache_max = 0;           /* maximum n that has been computed (1‑based) */

/* 12 initial terms, 1‑based index -> array index 0..11 */
static const seq_t init_vals[12] = {
    (seq_t)888,      (seq_t)1568,     (seq_t)3652,     (seq_t)7238,
    (seq_t)14338,    (seq_t)25576,    (seq_t)45156,    (seq_t)74150,
    (seq_t)120170,   (seq_t)184864,   (seq_t)280724,   (seq_t)410102
};

/* Grow cache to accommodate at least n entries */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < (size_t)n)
        new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p)
        exit(1);
    cache = p;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined by OEIS, return 0 for safety */

    /* Return from initial table if n <= 12 */
    if (n <= 12)
        return init_vals[n-1];

    /* Return cached value if already computed */
    if (n <= cache_max)
        return cache[n-1];

    /* Compute missing values up to n */
    ensure_cache(n);

    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        if (i <= 12) {          /* should not happen after the first step */
            cache[i-1] = init_vals[i-1];
            continue;
        }

        /* Retrieve previous 11 terms (i-1 down to i-11) */
        const seq_t a1 = cache[i-2];
        const seq_t a2 = cache[i-3];
        const seq_t a3 = cache[i-4];
        const seq_t a4 = cache[i-5];
        const seq_t a5 = cache[i-6];
        const seq_t a6 = cache[i-7];
        const seq_t a7 = cache[i-8];
        const seq_t a8 = cache[i-9];
        const seq_t a9 = cache[i-10];
        const seq_t a10 = cache[i-11];
        const seq_t a11 = cache[i-12];

        /* Compute the recurrence using signed 64‑bit arithmetic.
         * All intermediate results fit into signed 64‑bit for the values
         * that are expected to fit into the target seq_t type (for
         * 64‑bit outputs). */
        long long val =
              3LL * (long long)a1
            + 1LL * (long long)a2
            -11LL * (long long)a3
            + 6LL * (long long)a4
            +14LL * (long long)a5
            -14LL * (long long)a6
            - 6LL * (long long)a7
            +11LL * (long long)a8
            - 1LL * (long long)a9
            - 3LL * (long long)a10
            + 1LL * (long long)a11;

        cache[i-1] = (seq_t)val;
    }

    cache_max = n;
    return cache[n-1];
}
