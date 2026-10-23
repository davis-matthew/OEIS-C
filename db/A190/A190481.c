/*
 * A190481  Number of distinct integers with n digits which are the image of
 *          integers by the function Reverse and Add!.
 * https://oeis.org/A190481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A190481: Number of distinct n‑digit integers that are obtainable by the
   Reverse‑and‑Add function.
   The sequence satisfies the linear recurrence of order 9

        a(n) + a(n‑1) -21 a(n‑2) -21 a(n‑3)
              +35 a(n‑4) +35 a(n‑5) +56 a(n‑6)
              +56 a(n‑7) +19 a(n‑8) +19 a(n‑9) = p(n),

   where the inhomogeneous term p(n) (non‑zero only for n = 10) is
   p(10) = –9  (and p(n) = 0 for n > 10).
   Initial terms (n ≥ 1):

        a(1)=4, a(2)=14, a(3)=93, a(4)=256, a(5)=1793,
        a(6)=4872, a(7)=34107, a(8)=92590, a(9)=648154.
   For n ≥ 10 the recurrence above is used directly.
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef SEQ_T
/* The build supplies a type called seq_t.  Include its definition
   through the build system.  The code below works with any unsigned
   integral type or arbitrary‑precision integer class that supports
   the usual arithmetic operators. */
#endif

seq_t a(idx_t n)
{
    /*------------------------------------------------------------------
      Cache of already computed terms.  The array is indexed from 0
      (unused) up to the largest n that has been requested so far.
      ---------------------------------------------------------------*/
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;
    static idx_t last_computed = 0;

    /*------------------------------------------------------------------
      Initial values for 1 ≤ n ≤ 9
      ---------------------------------------------------------------*/
    static const seq_t init[10] = {
        0ULL,          /* a(0) – dummy */
        4ULL,          /* a(1) */
        14ULL,         /* a(2) */
        93ULL,         /* a(3) */
        256ULL,        /* a(4) */
        1793ULL,       /* a(5) */
        4872ULL,       /* a(6) */
        34107ULL,      /* a(7) */
        92590ULL,      /* a(8) */
        648154ULL      /* a(9) */
    };

    /*------------------------------------------------------------------
      Grow the cache if the requested n is outside the current range.
      ---------------------------------------------------------------*/
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();          /* out of memory */
        cache = tmp;
        cache_size = new_size;
    }

    /*------------------------------------------------------------------
      Compute terms that have not yet been cached.
      ---------------------------------------------------------------*/
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            if (i <= 9) {
                cache[i] = init[i];
            } else {
                /* positive coefficient contributions */
                seq_t pos =
                    cache[i - 1]                                    /* +1  */
                  + (seq_t)35 * cache[i - 4]                        /* +35 */
                  + (seq_t)35 * cache[i - 5]                        /* +35 */
                  + (seq_t)56 * cache[i - 6]                        /* +56 */
                  + (seq_t)56 * cache[i - 7]                        /* +56 */
                  + (seq_t)19 * cache[i - 8]                        /* +19 */
                  + (seq_t)19 * cache[i - 9];                       /* +19 */

                /* negative coefficient contributions */
                seq_t neg =
                    (seq_t)21 * cache[i - 2]                         /* -21 */
                  + (seq_t)21 * cache[i - 3];                       /* -21 */

                /* a(i) = neg - pos  (+ inhomogeneous part for i==10) */
                seq_t val = neg - pos;
                if (i == 10) val -= (seq_t)9;                      /* p_10 = -9 */

                cache[i] = val;
            }
        }
        last_computed = n;
    }

    return (n >= 1) ? cache[n] : (seq_t)0;
}
