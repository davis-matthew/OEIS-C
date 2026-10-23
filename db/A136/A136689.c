/*
 * A136689  Triangular sequence of q-Fibonacci polynomials for s=3: F(x,n) =
 *          x*F(x,n-1) + s*F(x,n-2).
 * https://oeis.org/A136689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the nth term of the triangle of q‑Fibonacci polynomials
 *            * for s = 3. The polynomials are defined by * F(x,0)=0 ,
 *            F(x,1)=1 * F(x,n)=x*F(x,n-1)+3*F(x,n-2) (n ≥ 2) * The
 *            coefficients of F(x,n) are enumerated row‑wise, with the first *
 *            coefficient of each row ranging from x⁰ up to xⁿ⁻¹. The s...
 * Range:     u8 n <= 38, u16 n <= 138, u32 n <= 442, u64 n <= 666,
 *            u128 n <= 666, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <new>
#include <cstddef>
#define ALLOC_T(N)               new seq_t[(N)]
#define FREE_T(P)                delete [] (P)
#else
#include <stdlib.h>
#define ALLOC_T(N)               ((seq_t*)malloc((N)*sizeof(seq_t)))
#define FREE_T(P)                free(P)
#endif

/*---------------------------------------------------------------*/
/*      Cache and static state used across all calls              */
/*---------------------------------------------------------------*/
static seq_t *cache      = NULL;   /* linear cache of all a(n) values */
static idx_t cache_len   = 0;      /* number of cached terms          */
static idx_t max_row     = 0;      /* largest row that has been built */
static seq_t *prev2      = NULL;   /* row r-2 (may be NULL when r<3)  */
static seq_t *prev1      = NULL;   /* row r-1                          */
static idx_t len2        = 0;       /* length of prev2                  */
static idx_t len1        = 0;       /* length of prev1                  */

/*---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /*---  Compute the row index that contains position n  ---*/
    idx_t r_needed = 1;
    idx_t cumulative = 1;                 /* rows 1..r_needed summed */
    while (cumulative < n) {
        ++r_needed;
        cumulative += r_needed;
    }

    /*---  Build rows up to r_needed if required  ---*/
    while (max_row < r_needed) {
        idx_t r_cur  = max_row + 1;        /* new row number          */
        idx_t len_cur = r_cur;             /* row length equals r_cur   */

        /* Allocate memory for the new row.  In C++ the array
           constructors are invoked automatically. */
        seq_t *curr = ALLOC_T(len_cur);
        if (!curr) {                      /* allocation failed */
            return 0;
        }

        /* Fill the coefficients of F(x,r_cur).   */
        if (r_cur == 1) {
            curr[0] = 1;                   /* F(x,1)=1                 */
        } else {
            for (idx_t k = 0; k < len_cur; ++k) {
                seq_t a_part = 0, b_part = 0;
                if (k >= 1 && (k - 1) < len1) {
                    a_part = prev1[k - 1];               /* x*F(n-1) part */
                }
                if (k < len2) {
                    b_part = prev2[k] * 3;               /* 3*F(n-2) part */
                }
                curr[k] = a_part + b_part;
            }
        }

        /* Append the new row to the linear cache.  For C++ we
           create a fresh array, copy each element (invoking copy
           assignment), and delete the old cache.  For C we use
           realloc/ memcpy.                                         */
        seq_t *new_cache = NULL;
#ifdef __cplusplus
        new_cache = ALLOC_T(cache_len + len_cur);
        if (!new_cache) {              /* allocation failure      */
            FREE_T(curr);
            return 0;
        }
        for (idx_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];   /* copy previous cache     */
        }
        for (idx_t i = 0; i < len_cur; ++i) {
            new_cache[cache_len + i] = curr[i]; /* copy new row      */
        }
        FREE_T(cache);                 /* delete old cache if any */
#else
        new_cache = (seq_t *)realloc(cache,
                                     (cache_len + len_cur) * sizeof(seq_t));
        if (!new_cache) {
            FREE_T(curr);
            return 0;
        }
        memcpy(new_cache + cache_len, curr, len_cur * sizeof(seq_t));
#endif
        cache = new_cache;              /* adopt new cache          */
        cache_len += len_cur;           /* update length            */

        /*---  Maintain the sliding window of last two rows  ---*/
        FREE_T(prev2);                  /* free the oldest row      */
        prev2 = prev1;                  /* advance window           */
        len2  = len1;
        prev1 = curr;                   /* new most recent row      */
        len1  = len_cur;

        max_row = r_cur;                /* update global row count   */
    }

    /*---  Retrieve the requested term  ---*/
    return cache[n - 1];                 /* cache is 0‑based         */
}
