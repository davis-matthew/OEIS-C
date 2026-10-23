/*
 * A140342  a(n)=5a(n-1)-11a(n-2)+13a(n-3)-9a(n-4)+3a(n-5)-a(n-6).
 * https://oeis.org/A140342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n)=5a(n-1)-11a(n-2)+13a(n-3)-9a(n-4)+3a(n-5)-a(n-6) * a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=1
 * Range:     i8 n <= 11, i16 n <= 20, i32 n <= 40, i64 n <= 76, i128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* memoised iterative evaluation of the linear recurrence.  The array
 * is grown only when a larger index is requested, irrespective of
 * the order of calls.  The implementation works for both C and C++:
 * in C++ it uses new/delete[] to preserve non‑trivial constructors,
 * in C it falls back to realloc/free.  All arithmetic is performed
 * in seq_t. */
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

static seq_t *cache = NULL;          /* pointers to computed values */
static size_t cache_len = 0;         /* number of valid entries */

/* Allocate an array of seq_t of the requested size.
 * For C++ we use new[]; for C we use realloc/free. */
static seq_t *alloc_arr(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)realloc(NULL, n * sizeof(seq_t));
#endif
}

/* Free an array allocated by alloc_arr().
 * In C++ we use delete[]; in C we use free. */
static void free_arr(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Extend the cache up to 'n' inclusive, computing missing terms. */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_len) return;   /* already computed */

    /* Allocate new cache */
    seq_t *new_cache = alloc_arr(need);
    if (!new_cache) abort();          /* out of memory */

    /* Copy existing values */
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];

    /* Base values for n=0..5 */
    const seq_t base[6] = { (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)1 };

    /* Compute any terms beyond the base. */
    for (size_t i = cache_len; i < need; ++i) {
        if (i < 6) {
            new_cache[i] = base[i];
        } else {
            new_cache[i] =
                (5 * new_cache[i - 1]    )
              - (11 * new_cache[i - 2]   )
              + (13 * new_cache[i - 3]   )
              - (9  * new_cache[i - 4]   )
              + (3  * new_cache[i - 5]   )
              - (new_cache[i - 6]          );
        }
    }

    /* Replace old cache */
    free_arr(cache);
    cache = new_cache;
    cache_len = need;
}

/* Public function giving the nth term of the sequence.  */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
