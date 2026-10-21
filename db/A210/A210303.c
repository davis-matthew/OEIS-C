/*
 * A210303  Number of (n+1)X3 0..3 arrays containing all values 0..3 with
 *          every 2X2 subblock having one or three distinct values, and new
 *          values 0..3 introduced in row major order.
 * https://oeis.org/A210303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 35*a(n-1) - 315*a(n-2) + 729*a(n-3) + 1132*a(n-4) -
 *            3804*a(n-5) + 688*a(n-6) + 1536*a(n-7) Initial values (OEIS
 *            A210303): a(1)=29, a(2)=1091, a(3)=27957, a(4)=655743,
 *            a(5)=14976529, a(6)=339125187, a(7)=7657326365
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* cache[0] unused; cache[i] = a(i) */
static idx_t  cache_limit = 0;     /* number of elements allocated */
static idx_t  computed_upto = 0;   /* highest n computed so far */

/* Ensure cache has space for indices 0..n */
static void ensure_cache_size(idx_t n)
{
    if (cache_limit <= (size_t)n)
    {
        size_t new_limit = n + 1;
        if (new_limit < 8)        /* at least for the 7 base terms */
            new_limit = 8;
        if (cache == NULL)
            cache = (seq_t*)malloc(new_limit * sizeof(seq_t));
        else
            cache = (seq_t*)realloc(cache, new_limit * sizeof(seq_t));
        for (size_t i = cache_limit; i < new_limit; ++i)
            cache[i] = 0;
        cache_limit = new_limit;
    }
}

/* Initialise the seven known terms of the sequence */
static void init_base_terms(void)
{
    cache[1] = (seq_t)29;
    cache[2] = (seq_t)1091;
    cache[3] = (seq_t)27957;
    cache[4] = (seq_t)655743;
    cache[5] = (seq_t)14976529;
    cache[6] = (seq_t)339125187;
    cache[7] = (seq_t)7657326365;
    computed_upto = 7;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for n < 1 */

    ensure_cache_size(n);

    /* Initialise and compute up to n */
    if (computed_upto == 0)
        init_base_terms();

    while (computed_upto < n)
    {
        idx_t i = computed_upto + 1;
        /* Split the recurrence into positive and negative parts */
        seq_t pos = 35*cache[i-1] + 729*cache[i-3] + 1132*cache[i-4]
                    + 688*cache[i-6] + 1536*cache[i-7];
        seq_t neg = 315*cache[i-2] + 3804*cache[i-5];
        cache[i] = pos - neg;              /* pos >= neg for this sequence */
        computed_upto = i;
    }

    return cache[n];
}
