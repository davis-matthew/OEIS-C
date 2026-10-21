/*
 * A090145  Even-indexed terms of the binomial transform equal 1 and the
 *          odd-indexed terms of the second binomial transform equal 1.
 * https://oeis.org/A090145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 9, i32 n <= 15, i64 n <= 23, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0) = 1
   a(n) = 1 - 2^n                     if n is odd
   a(n) = 1 - sum_{k=0}^{n-1} C(n,k) * a(k)
          (C(n,n)=1)                  if n is even
   where C(n,k) are binomial coefficients.
   This satisfies the definition:
   b(2m)=sum_{k=0}^{2m} C(2m,k)a(k)=1  and
   c(2m+1)=sum_{k=0}^{2m+1} 2^{2m+1-k}C(2m+1,k)a(k)=1.
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- static cache ---------- */
    static seq_t *A = NULL;   /* sequence values a(0)..a(cache_len-1)   */
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* ---------- power of two for odd indices ----------
       pow2 holds 2^i for the current index i.             */
    static seq_t pow2 = 1;    /* 2^0 = 1 */
    static idx_t pow2_index = 0; /* current index for pow2 */

    if (cache_len <= 1) {
        /* ensure A[0] exists */
        if (cache_cap < 1) {
            cache_cap = 1;
            A = (seq_t*)realloc(A, cache_cap * sizeof(seq_t));
        }
        if (cache_len == 0) {
            A[0] = 1;
            cache_len = 1;
        }
    }

    /* extend cache up to n */
    while (cache_len <= (size_t)n) {
        idx_t i = (idx_t)cache_len;  /* next index to compute */

        /* advance power of two for odd indices */
        if (i > 0)
            pow2 = pow2 * 2;    /* 2^i                                           */
        pow2_index = i;

        /* compute a[i] */
        seq_t val;
        if ((i & 1) == 1) { /* odd */
            val = 1 - pow2;
        } else { /* even */
            seq_t sum = 0;
            seq_t binom = 1;  /* C(i,0) */
            for (idx_t k = 0; k < i; ++k) {
                sum = sum + binom * A[k];
                /* update binom to C(i, k+1) */
                binom = (binom * (i - k)) / (k + 1);
            }
            val = 1 - sum;   /* since C(i,i)=1 */
        }

        /* store value */
        if (cache_len >= cache_cap) {
            cache_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
            A = (seq_t*)realloc(A, cache_cap * sizeof(seq_t));
        }
        A[cache_len++] = val;
    }

    return A[n];
}
