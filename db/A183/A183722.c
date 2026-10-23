/*
 * A183722  1/12 the number of (n+1) X 3 0..5 arrays with every 2 X 2 subblock
 *          strictly increasing clockwise or counterclockwise with one
 *          decrease, and at least two adjacent blocks having the same
 *          increasing direction.
 * https://oeis.org/A183722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: 
   a(n) = 14*a(n-1)-36*a(n-2)-115*a(n-3)+125*a(n-4)+164*a(n-5)
          -155*a(n-6)-29*a(n-7)+34*a(n-8)+a(n-9)-2*a(n-10)
   initial terms: a(1)=1,a(2)=2,a(3)=29,a(4)=388,a(5)=4170,
   a(6)=41423,a(7)=388998,a(8)=3528126,a(9)=31206553,a(10)=270945278 */
#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[10] = {
    (seq_t)1,   (seq_t)2,   (seq_t)29,   (seq_t)388,
    (seq_t)4170, (seq_t)41423, (seq_t)388998, (seq_t)3528126,
    (seq_t)31206553, (seq_t)270945278
};

static seq_t *cache       = NULL; /* cache[0] corresponds to a(1) */
static size_t  cache_size = 0;    /* number of values stored   */
static size_t  cache_cap  = 0;    /* allocated capacity        */

/* Ensure cache contains a(1) … a(n) (n ≥ 1) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;

    /* Expand storage if necessary */
    if (cache_cap < (size_t)n) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap != 0) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Copy initial values up to min(n,10) */
    if (cache_size < 10) {
        size_t upto = (size_t)n < 10 ? (size_t)n : 10;
        for (size_t i = cache_size; i < upto; ++i)
            cache[i] = init_vals[i];
        cache_size = upto;
    }

    /* Compute further terms using the recurrence */
    for (idx_t i = (idx_t)(cache_size + 1); i <= n; ++i) {
        seq_t term = 0;
        /* positive contributions */
        term += 14 * cache[(size_t)(i - 2)];   /* a(i-1) */
        term += 125 * cache[(size_t)(i - 5)];  /* a(i-4) */
        term += 164 * cache[(size_t)(i - 6)];  /* a(i-5) */
        term += 34 * cache[(size_t)(i - 9)];   /* a(i-8) */
        term += cache[(size_t)(i - 10)];       /* a(i-9) */
        /* negative contributions */
        term -= 36 * cache[(size_t)(i - 3)];   /* a(i-2) */
        term -= 115 * cache[(size_t)(i - 4)];  /* a(i-3) */
        term -= 155 * cache[(size_t)(i - 7)];  /* a(i-6) */
        term -= 29 * cache[(size_t)(i - 8)];   /* a(i-7) */
        term -= 2 * cache[(size_t)(i - 11)];   /* a(i-10) */

        cache[cache_size++] = term;
    }
}

extern seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, safe fallback */
    ensure_cache(n);
    return cache[n-1];
}
