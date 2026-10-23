/*
 * A016849  Expansion of 1/((1-3x)(1-4x)(1-8x)).
 * https://oeis.org/A016849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 68*a(n-2) + 96*a(n-3), with initial values
 *            a(0)=1, a(1)=15, a(2)=157
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#if defined(__cplusplus)
#include <new>
#endif

static seq_t *cache = NULL;   /* array of sequence terms */
static size_t cap = 0;        /* allocated size of cache */
static size_t len = 0;        /* number of terms currently stored */

/* allocate a new array of seq_t of given size */
static seq_t* alloc_array(size_t n)
{
#if defined(__cplusplus)
    return new seq_t[n];
#else
    return (seq_t*)malloc(n * sizeof(seq_t));
#endif
}

/* free an array of seq_t */
static void free_array(seq_t *p)
{
#if defined(__cplusplus)
    delete [] p;
#else
    free(p);
#endif
}

/* reallocate an array of seq_t, preserving existing data */
static seq_t* reallocate(seq_t *p, size_t old_cap, size_t new_cap)
{
#if defined(__cplusplus)
    seq_t *newp = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        newp[i] = p[i];
    delete [] p;
    return newp;
#else
    return (seq_t*)realloc(p, new_cap * sizeof(seq_t));
#endif
}

/* Compute a(n) for the OEIS sequence A016849. */
seq_t a(idx_t n)
{
    /* Ensure cache capacity up to n+1 terms. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (cap < 3) ? 3 : cap;
        while ((size_t)n + 1 > new_cap)
            new_cap *= 2;
        cache = reallocate(cache, cap, new_cap);
        cap = new_cap;
    }

    /* Initialize first three terms if not already set. */
    if (len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)15;
        cache[2] = (seq_t)157;
        len = 3;
    } else if (len == 1) {
        cache[1] = (seq_t)15;
        cache[2] = (seq_t)157;
        len = 3;
    } else if (len == 2) {
        cache[2] = (seq_t)157;
        len = 3;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        /* Use recurrence for i >= 3. */
        cache[i] = ( (seq_t)15 * cache[i-1] )
                 - ( (seq_t)68 * cache[i-2] )
                 + ( (seq_t)96 * cache[i-3] );
    }

    len = (size_t)n + 1;
    return cache[n];
}
