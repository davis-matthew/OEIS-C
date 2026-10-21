/*
 * A055836  T(2n+2, n), where T is the array in A055830.
 * https://oeis.org/A055836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=2, a(1)=7;
 * For n>=2:
 *   5*n*(n+1)*(7*n-5)*a(n) =
 *     n*(154*n^2+2*n-77)*a(n-1) + 3*(3*n-4)*(7*n+2)*(3*n-2)*a(n-2)   */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cache of previously computed terms */
    static seq_t *cache = NULL;         /* array of a(i)   */
    static idx_t cache_size = 0;        /* number of stored entries */
    static idx_t cache_capacity = 0;    /* allocated capacity      */

    /* first call: allocate and initialise */
    if (cache == NULL) {
        cache_capacity = 2;
        cache = (seq_t*)malloc(cache_capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed */
        cache[0] = (seq_t)2;          /* a(0) */
        cache[1] = (seq_t)7;          /* a(1) */
        cache_size = 2;
    }

    /* grow the cache if necessary */
    if (n >= cache_capacity) {
        idx_t new_capacity = cache_capacity * 2;
        if (new_capacity <= n) new_capacity = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_capacity * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failed */
        cache = tmp;
        cache_capacity = new_capacity;
    }

    /* compute terms up to n */
    while (cache_size <= n) {
        idx_t m = cache_size;          /* current n */

        /* use big‑integer arithmetic when seq_t is big, otherwise use 128‑bit */
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            /* 64‑bit build: use 128‑bit intermediate */
            unsigned __int128 nm = (unsigned __int128)m;
            unsigned __int128 coeff1 = nm *
                ((unsigned __int128)154 * nm * nm + (unsigned __int128)2 * nm - (unsigned __int128)77);
            unsigned __int128 coeff2 = (unsigned __int128)3 *
                ((unsigned __int128)3 * nm - 4) *
                ((unsigned __int128)7 * nm + 2) *
                ((unsigned __int128)3 * nm - 2);
            unsigned __int128 denom  = (unsigned __int128)5 * nm *
                                       (nm + 1) *
                                       ((unsigned __int128)7 * nm - 5);

            unsigned __int128 term1 = (unsigned __int128)cache[m-1] * coeff1;
            unsigned __int128 term2 = (unsigned __int128)cache[m-2] * coeff2;
            unsigned __int128 numer  = term1 + term2;
            seq_t val = (seq_t)(numer / denom);
            cache[cache_size++] = val;
        } else {
            /* arbitrary‑precision build: all operations stay exact */
            seq_t nm = (seq_t)m;
            seq_t coeff1 = nm * ( (seq_t)154 * nm * nm + (seq_t)2 * nm - (seq_t)77 );
            seq_t coeff2 = (seq_t)3 * ( (seq_t)3 * nm - (seq_t)4 )
                                     * ( (seq_t)7 * nm + (seq_t)2 )
                                     * ( (seq_t)3 * nm - (seq_t)2 );
            seq_t denom  = (seq_t)5 * nm * (nm + (seq_t)1)
                                   * ( (seq_t)7 * nm - (seq_t)5 );

            seq_t term1 = cache[m-1] * coeff1;
            seq_t term2 = cache[m-2] * coeff2;
            seq_t numer = term1 + term2;
            seq_t val   = numer / denom;
            cache[cache_size++] = val;
        }
    }

    return cache[n];
}
