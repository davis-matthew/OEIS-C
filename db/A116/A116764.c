/*
 * A116764  Number of permutations of length n which avoid the patterns 1423,
 *          2134, 3214.
 * https://oeis.org/A116764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 17*a(n-2) + 19*a(n-3) - 9*a(n-4) for n ≥ 5;
 *            initial values: a(1)=1, a(2)=2, a(3)=6, a(4)=21
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, free */
#ifdef __cplusplus
#include <new>       /* operator new, delete[] */
#endif

/* internal helper to allocate a seq_t array of given capacity */
static seq_t *alloc_seq(size_t cap)
{
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t *)malloc(cap * sizeof(seq_t));
#endif
}

/* internal helper to reallocate seq_t array to new capacity */
static void reallocate_seq(seq_t **arr, size_t *cap, size_t new_cap)
{
    seq_t *new_arr = alloc_seq(new_cap);
    if (*arr != NULL) {
        /* copy existing data */
        for (size_t i = 0; i < *cap; ++i)
            new_arr[i] = (*arr)[i];
#ifdef __cplusplus
        delete[] *arr;
#else
        free(*arr);
#endif
    }
    *arr = new_arr;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* static memoisation: seq[0] unused; seq[1] holds a(1), etc. */
    static seq_t *seq = NULL;      /* pointer to stored terms */
    static size_t cap = 0;         /* allocated capacity (>= max_n+1) */
    static idx_t  max_n = 0;        /* highest n that has been computed */

    if (n == 0)   /* the sequence is defined for n ≥ 1 */
        return (seq_t)0;

    /* ensure enough capacity */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (cap > 0) ? cap : 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;          /* grow geometrically */
        reallocate_seq(&seq, &cap, new_cap);
    }

    /* compute missing terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i == 1)
            seq[i] = (seq_t)1;
        else if (i == 2)
            seq[i] = (seq_t)2;
        else if (i == 3)
            seq[i] = (seq_t)6;
        else if (i == 4)
            seq[i] = (seq_t)21;
        else
            seq[i] =  7 * seq[i-1]
                    - 17 * seq[i-2]
                    + 19 * seq[i-3]
                    -  9 * seq[i-4];
    }
    max_n = n;

    return seq[n];
}
