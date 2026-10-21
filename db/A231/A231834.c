/*
 * A231834  Number of nX3 0..3 arrays with no element less than a strict
 *          majority of its horizontal and antidiagonal neighbors.
 * https://oeis.org/A231834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: 
 * a(n) = 16*a(n-1) + 206*a(n-2) + 762*a(n-3) - 3181*a(n-4) - 9838*a(n-5)
 *        +4473*a(n-6) + 18193*a(n-7) + 2848*a(n-8) - 6404*a(n-9) - 1848*a(n-10)
 *   for n > 11,
 * initial terms:
 * a(1) = 16, a(2) = 188, a(3) = 4508, a(4) = 111621, a(5) = 2836554,
 * a(6) = 71178861, a(7) = 1792092360, a(8) = 45099279326,
 * a(9) = 1134900171250, a(10) = 28560684486812, a(11) = 718739864735561.
 */ 

#include <stdlib.h> 
/* no other headers required */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* allocated elements */
    static size_t size = 0;       /* number of terms already stored */

    /* pre‑known first 11 terms */
    static const seq_t init_vals[11] = {
        (seq_t)16ULL,
        (seq_t)188ULL,
        (seq_t)4508ULL,
        (seq_t)111621ULL,
        (seq_t)2836554ULL,
        (seq_t)71178861ULL,
        (seq_t)1792092360ULL,
        (seq_t)45099279326ULL,
        (seq_t)1134900171250ULL,
        (seq_t)28560684486812ULL,
        (seq_t)718739864735561ULL
    };

    /* coefficients of the recurrence */
    static const int coeff[10] = {
        16,  206,  762,  -3181, -9838,
        4473, 18193, 2848, -6404, -1848
    };

    /* Allocate cache the first time */
    if (cache == NULL) {
        size_t init_cap = (size_t)(n > 11 ? n : 11);
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed; caller ignores */
        capacity = init_cap;
        size = 0;
    }

    /* Grow cache if necessary */
    if ((size_t)n > capacity) {
        size_t new_cap = capacity;
        while (new_cap < (size_t)n) new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;           /* allocation failed; caller ignores */
        cache = tmp;
        capacity = new_cap;
    }

    /* initialise the first 11 terms on first use (or for a call with n < 11) */
    if (size < 11) {
        size_t limit = (size_t)(11 < n ? 11 : n);
        for (idx_t i = (idx_t)size; i < limit; ++i) {
            cache[(size_t)i] = init_vals[(size_t)i];
        }
        size = limit;
    }

    /* compute further terms using the recurrence */
    while ((size_t)size < (size_t)n) {
        /* compute a(size+1) which is stored at cache[size] */
        seq_t pos = 0;
        seq_t neg = 0;
        for (int j = 0; j < 10; ++j) {
            int c = coeff[j];
            if (c > 0) {
                pos += (seq_t)c * cache[(size_t)(size - (j + 1))];
            } else {
                neg += (seq_t)(-c) * cache[(size_t)(size - (j + 1))];
            }
        }
        /* pos >= neg for all terms of this sequence, so unsigned subtraction is safe */
        cache[(size_t)size] = pos - neg;
        ++size;
    }

    return cache[(size_t)(n - 1)];
}
