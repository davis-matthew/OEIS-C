/*
 * A220648  Number of ways to reciprocally link elements of an nX3 array
 *          either to themselves or to exactly two king-move neighbors,
 *          without 3-loops.
 * https://oeis.org/A220648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence: A220648 
   Number of ways to reciprocally link elements of an n×3 array either 
   to themselves or to exactly two king‑move neighbors, without 3‑loops. 
   Recurrence: 
     a(n) =  9*a(n-1) + 55*a(n-2) +119*a(n-3) -44*a(n-4) -388*a(n-5) 
           -58*a(n-6) +355*a(n-7) +79*a(n-8) -85*a(n-9) -13*a(n-10) 
   Initial values (n = 1 … 10): 
     1, 23, 333, 4352, 59846, 816159, 11131560, 151872860, 
     2071881506, 28265332768
*/

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;             /* undefined for n<1 */

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cached_to = 0;             /* highest n cached so far */
    static size_t cache_sz = 0;             /* allocated size (index 0 unused) */

    /* Initial 10 terms (index 1‑based).  These are required by the 
       recurrence; they are the first values that can be computed directly. */
    static const seq_t init[10] = {
        (seq_t)1,
        (seq_t)23,
        (seq_t)333,
        (seq_t)4352,
        (seq_t)59846,
        (seq_t)816159,
        (seq_t)11131560,
        (seq_t)151872860,
        (seq_t)2071881506,
        (seq_t)28265332768
    };

    /* Recurrence coefficients.  Positive numbers correspond to addition, 
       negative numbers to subtraction. */
    static const int coeff[10] = {
        9,   55,  119, -44, -388, -58,  355,  79, -85, -13
    };

    /* Ensure sufficient cache size. */
    if (cache_sz <= n) {
        size_t new_sz = cache_sz > 0 ? cache_sz : 1;
        while (new_sz <= (size_t)n) new_sz <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_sz + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failed */
        cache = tmp;
        cache_sz = new_sz;
    }

    /* Compute missing values up to n. */
    for (idx_t i = cached_to + 1; i <= n; ++i) {
        if (i <= 10) {
            cache[i] = init[i - 1];
            continue;
        }
        seq_t sum = 0;
        for (int k = 0; k < 10; ++k) {
            seq_t term = cache[i - 1 - k];
            int c = coeff[k];
            if (c >= 0)
                sum += (seq_t)c * term;
            else
                sum -= (seq_t)(-c) * term;
        }
        cache[i] = sum;
    }
    cached_to = n;
    return cache[n];
}
