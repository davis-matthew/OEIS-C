/*
 * A200870  Number of 0..7 arrays x(0..n+1) of n+2 elements without any
 *          interior element greater than both neighbors or less than both
 *          neighbors.
 * https://oeis.org/A200870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) … a(11) = 232, 932, 3704, 14932, 60112, 241718, 971416,
 *            3904290, 15693816, 63085186, 253583656 a(n) = 4*a(n-1) -
 *            2*a(n-2) + 4*a(n-3) + 15*a(n-4) + 6*a(n-5) + 12*a(n-6) +
 *            16*a(n-7) + 7*a(n-8) + 5*a(n-9) + 4*a(n-10) + a(n-11) (n ≥ 12)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[1] … cache[n] */
static size_t cache_cap = 0;     /* current capacity of cache */

/* Multiply a seq_t value by a small unsigned constant.
   The constant is at most 16, so a loop is cheap and works for all seq_t types. */
static seq_t mul_const(seq_t a, unsigned c)
{
    seq_t r = 0;
    for (unsigned i = 0; i < c; ++i)
        r += a;
    return r;
}

/* Ensure that the cache has room for indices 1 … n.  All entries are
   zero-initialised on first allocation. */
static void ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n) return;

    size_t new_cap = (size_t)n + 1; /* 1‑based index: cache[0] is unused */
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();              /* allocation failed */
    /* Initialise any new elements to zero.  For integral types this is fine;
       big‑integer types are assumed to have a trivial zero representation. */
    for (size_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = 0;
    cache = tmp;
    cache_cap = new_cap;
}

/* Initial values for n = 1 … 11.  They are stored in 1‑based form for clarity. */
static const seq_t initvals[11] = {
    232ULL,      932ULL,      3704ULL,
    14932ULL,    60112ULL,    241718ULL,
    971416ULL,   3904290ULL, 15693816ULL,
    63085186ULL, 253583656ULL
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;                           /* out‑of‑range */

    if (n <= 11) return initvals[(size_t)n - 1];

    /* Allocate cache if needed. */
    ensure_cache(n);

    /* Initialise cache[1] … cache[11] only once. */
    if (cache[1] == 0) {
        for (size_t i = 1; i <= 11; ++i)
            cache[i] = initvals[i - 1];
    }

    /* Compute successive terms up to n. */
    for (size_t i = 12; i <= (size_t)n; ++i) {
        /* Forward calculation ensures the positive sum is computed
           before the subtraction, avoiding underflow for unsigned types. */
        seq_t val = mul_const(cache[i - 1], 4);
        val += mul_const(cache[i - 3], 4);
        val += mul_const(cache[i - 4], 15);
        val += mul_const(cache[i - 5], 6);
        val += mul_const(cache[i - 6], 12);
        val += mul_const(cache[i - 7], 16);
        val += mul_const(cache[i - 8], 7);
        val += mul_const(cache[i - 9], 5);
        val += mul_const(cache[i -10], 4);
        val += cache[i -11];
        val -= mul_const(cache[i -2], 2);

        cache[i] = val;
    }

    return cache[(size_t)n];
}
