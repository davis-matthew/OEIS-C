/*
 * A130145  Number of nonisomorphic orthogonal arrays OA(8*n+4,4,2,2).
 * https://oeis.org/A130145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A130145: Number of nonisomorphic orthogonal arrays OA(8*n+4,4,2,2).
   Recurrence (obtained from the rational generating function
   x / ((x-1)^6*(x+1)*(x^2+x+1)^2)):
     a(1) = 1
     a(2) = 3
     a(3) = 7
     a(4) = 15
     a(5) = 28
     a(6) = 48
     a(7) = 79
     a(8) = 123
     a(9) = 184
     a(10)= 268
     a(11)= 379
     for n > 11:
        a(n) =   3*a(n-1)
               - 2*a(n-2)
               - 3*a(n-4)
               + 3*a(n-5)
               + 3*a(n-6)
               - 3*a(n-7)
               - 2*a(n-9)
               + 3*a(n-10)
               -   a(n-11)
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms a(1)..a(11) */
    static const seq_t init_terms[11] = {
        1ULL, 3ULL, 7ULL, 15ULL, 28ULL, 48ULL, 79ULL,
        123ULL, 184ULL, 268ULL, 379ULL
    };

    /* Coefficients for the recurrence:
       coefficient for a(n-i-1) is coeffs[i] (i=0..10). */
    static const int coeffs[11] = {
        3,   /*  a(n-1)   */
       -2,   /*  a(n-2)   */
        0,   /*  a(n-3)   */
       -3,   /*  a(n-4)   */
        3,   /*  a(n-5)   */
        3,   /*  a(n-6)   */
       -3,   /*  a(n-7)   */
        0,   /*  a(n-8)   */
       -2,   /*  a(n-9)   */
        3,   /*  a(n-10)  */
       -1    /*  a(n-11)  */
    };

    /* Cache for memoised results */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number values stored (a(1)..a(cache_len)) */

    if (n == 0)            /* OEIS offset starts at 1; undefined value for n=0 */
        return 0;

    /* Expand cache to hold values up to n */
    if (n > cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!new_cache)
            abort();      /* memory allocation failure */
        cache = new_cache;

        /* Fill new entries in ascending order */
        for (idx_t m = cache_len + 1; m <= n; ++m) {
            if (m <= 11) {                   /* use pre‑computed values */
                cache[m - 1] = init_terms[m - 1];
                continue;
            }

            /* Apply linear recurrence (order 11) */
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            for (int i = 0; i < 11; ++i) {
                int c = coeffs[i];
                if (c == 0) continue;

                /* Index of a(m-i-1) in cache: (m-i-2) because cache[0] = a(1) */
                seq_t term = cache[m - i - 2];

                if (c > 0) {
                    sum_pos += (seq_t)c * term;
                } else {
                    sum_neg += (seq_t)(-c) * term;
                }
            }
            cache[m - 1] = sum_pos - sum_neg;   /* sum_pos >= sum_neg for this sequence */
        }
        cache_len = n;
    }

    return cache[n - 1];
}
