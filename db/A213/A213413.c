/*
 * A213413  Half the number of n X 3 binary arrays with no 3 X 3 submatrix
 *          formed with any three rows and columns equal to J-I.
 * https://oeis.org/A213413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5^n - 3*6^n + 3*7^n)/2 Recurrence: a(n) = 18*a(n-1) -
 *            107*a(n-2) + 210*a(n-3) Initial values: a(1)=4, a(2)=32,
 *            a(3)=253
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* memoisation cache: a(1), a(2), a(3), ... */
static seq_t *cache = NULL;      /* array of seq_t values */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static size_t cache_size = 0;    /* number of elements actually stored */

/* compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* invalid index, return safe value */

    /* initialise cache with first three terms if necessary */
    if (cache == NULL) {
        cache_cap = 3;
        cache_size = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;  /* allocation failure */
        cache[0] = (seq_t)4;   /* a(1) */
        cache[1] = (seq_t)32;  /* a(2) */
        cache[2] = (seq_t)253; /* a(3) */
    }

    /* if requested term is already cached, return it */
    if (cache_size >= (size_t)n)
        return cache[n - 1];

    /* extend the cache up to term n using the recurrence */
    for (idx_t i = (idx_t)cache_size + 1; i <= n; ++i) {
        /* ensure enough capacity */
        if ((size_t)i > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            while (new_cap < (size_t)i) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) {        /* allocation failure */
                free(cache);
                cache = NULL;
                cache_cap = cache_size = 0;
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* indices are zero‑based in the cache */
        seq_t a_nm1 = cache[i - 2];  /* a(n-1) */
        seq_t a_nm2 = cache[i - 3];  /* a(n-2) */
        seq_t a_nm3 = cache[i - 4];  /* a(n-3) */

        seq_t val = a_nm1 * (seq_t)18;
        val -= a_nm2 * (seq_t)107;
        val += a_nm3 * (seq_t)210;

        cache[i - 1] = val;
    }

    cache_size = (size_t)n;
    return cache[n - 1];
}
