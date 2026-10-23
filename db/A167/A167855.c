/*
 * A167855  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=45, a(2)=1980,..., a(14)=104255099738019625697280.
 *            For n>15: a(n) = 43*(a(n-1)+...+a(n-14)) - 946*a(n-15). For
 *            n=15: a(15) = 1 + 43*(a(14)+...+a(1)) - 946*a(0)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   mul_small, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime sum generator, use linear recurrence of order 15.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"   /* seq_t seq_from_str(const char *); */
#include "../utils/mul_small.h"     /* seq_t mul_small(seq_t, unsigned int); */

static const char *init_terms_str[15] = {
    "1",
    "45",
    "1980",
    "87120",
    "3833280",
    "168664320",
    "7421230080",
    "326534123520",
    "14367501434880",
    "632170063134720",
    "27815482777927680",
    "1223881242228817920",
    "53850774658067988480",
    "2369434084954991493120",
    "104255099738019625697280"
};

static seq_t *cache = NULL;           /* holds computed values */
static size_t cache_cap = 0;           /* capacity of cache array */
static idx_t cache_n = (idx_t)-1;     /* largest index computed (inclusive) */

/* Allocate and initialise cache with the first 15 terms.  */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    cache_cap = 64;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (cache == NULL)
        abort();

    /* Load the first 15 terms from the string constants.  */
    for (size_t i = 0; i < 15; ++i)
        cache[i] = seq_from_str(init_terms_str[i]);

    cache_n = 14;
}

/* Grow the cache array if necessary to hold up to index n.  */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;
    while (cache_cap <= (size_t)n)
        cache_cap <<= 1;                 /* double capacity */
    seq_t *newp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (newp == NULL)
        abort();
    cache = newp;
}

/* Main sequence routine.  */
seq_t a(idx_t n)
{
    init_cache();

    if (n <= (idx_t)cache_n)
        return cache[n];

    /* Ensure cache is large enough for all needed indices.  */
    ensure_capacity(n);

    for (idx_t i = cache_n + 1; i <= n; ++i)
    {
        /* sum_{j=1}^{14} 43 * a(i-j) */
        seq_t sum = 0;
        for (int j = 1; j <= 14; ++j)
            sum += mul_small(cache[i - j], 43);

        /* subtract 946 * a(i-15) */
        seq_t last = mul_small(cache[i - 15], 946);
        seq_t val = sum - last;

        /* special adjustment for n = 15 (adds the constant 1) */
        if (i == 15)
            val += 1;

        cache[i] = val;
    }

    cache_n = n;
    return cache[n];
}
