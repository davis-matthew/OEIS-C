/*
 * A064184  Denominator of sequence defined by recursion c(n)=1+c(n-2)/c(n-1),
 *          c(0)=0, c(1)=1.
 * https://oeis.org/A064184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (a(n-1)*a(n-4) + a(n-2)^2) * a(n-2) / a(n-4) for n ≥ 4
 *            (a(0)=a(1)=a(2)=a(3)=1)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamically allocated array of terms          */
static size_t alloc_size = 0; /* number of slots currently allocated           */
static idx_t computed_size = 0;/* number of terms that have been fully computed */

static void ensure_alloc(idx_t n)
{
    /* Allocate space for indices up to n (inclusive) */
    if (alloc_size <= (size_t)n)
    {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        alloc_size = new_size;
    }
}

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)1;    /* base cases a(0)=a(1)=a(2)=a(3)=1 */

    ensure_alloc(n);

    /* Initialise the first four terms if not yet done */
    if (computed_size < 4)
    {
        cache[0] = cache[1] = cache[2] = cache[3] = (seq_t)1;
        computed_size = 4;
    }

    /* If the requested term is already known, return it */
    if (n < computed_size) return cache[n];

    /* Compute all missing terms up to and including n */
    for (idx_t i = computed_size; i <= n; ++i)
    {
        seq_t a1 = cache[i - 1];   /* a(i-1) */
        seq_t a2 = cache[i - 2];   /* a(i-2) */
        seq_t a4 = cache[i - 4];   /* a(i-4) */

        seq_t t1 = a1 * a4 + a2 * a2; /* a(i-1)*a(i-4) + a(i-2)^2 */
        seq_t t2 = t1 * a2;           /* multiply by a(i-2) */
        cache[i] = t2 / a4;           /* exact division */
    }

    computed_size = n + 1;
    return cache[n];
}
