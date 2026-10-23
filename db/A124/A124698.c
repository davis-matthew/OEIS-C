/*
 * A124698  Number of base 5 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) - 2*a(n-3) + 4*a(n-4) for n > 4, with
 *            initial values: a(0)=1, a(1)=5, a(2)=13, a(3)=29, a(4)=73
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values of the sequence */
static seq_t *cache   = NULL; /* array of values */
static idx_t  capacity = 0;   /* size of allocated array */
static idx_t  computed = 0;   /* number of values actually computed */

/* Ensure the cache has room for at least indices 0..n inclusively */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        (void) capacity;
        idx_t init_cap = 16;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();   /* allocation failed */
        capacity = init_cap;
        computed  = 0;
    }
    if (n < capacity) return;
    idx_t new_cap = capacity;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();        /* allocation failed */
    cache    = tmp;
    capacity = new_cap;
}

/* Return the n-th term of OEIS sequence A124698. */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    /* Compute all missing terms up to n. */
    for (idx_t i = computed; i <= n; ++i)
    {
        seq_t val;
        switch (i) {
            case 0: val = (seq_t)1;                break;
            case 1: val = (seq_t)5;                break;
            case 2: val = (seq_t)13;               break;
            case 3: val = (seq_t)29;               break;
            case 4: val = (seq_t)73;               break;
            default:
            {
                /* a(i) = 5*a(i-1) - 6*a(i-2) - 2*a(i-3) + 4*a(i-4) */
                seq_t sum1 = cache[i-1] * (seq_t)5;
                sum1 += cache[i-4] * (seq_t)4;
                seq_t sum2 = cache[i-2] * (seq_t)6;
                sum2 += cache[i-3] * (seq_t)2;
                val = (sum1 >= sum2) ? (sum1 - sum2) : 0; /* should not happen */
            }
        }
        cache[i] = val;
    }
    computed = n + 1;
    return cache[n];
}
