/*
 * A170342  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence derived from the generating function:   
   a(0) = 1;   
   For 1 ≤ n ≤ 41: a(n) = 43*∑_{i=1}^{n} a(n-i) + 2;   
   For n = 42:     a(42) = 43*∑_{i=1}^{41} a(42-i) + 1;   
   For n > 42:     a(n) = 43*∑_{i=1}^{41} a(n-i) – 946*a(n-42).   
   These rules reproduce all terms of A170342.    
*/
#include <stdlib.h>

#if defined(__cplusplus)
    /* In C++ use dynamic array constructed with new[] */
    #define ALLOC_ARRAY(type, len, ptr)  (ptr) = new type[(size_t)(len)]
    #define FREE_ARRAY(ptr, len)         { delete[] (ptr); (ptr) = NULL; }
#else
    /* In C use malloc/free (no construction needed for integer types) */
    #define ALLOC_ARRAY(type, len, ptr)  (ptr) = (type*)realloc(NULL, (size_t)(len) * sizeof(type))
    #define FREE_ARRAY(ptr, len)         { free(ptr); (ptr) = NULL; }
#endif

seq_t a(idx_t n)
{
    /* allocate an array of size n+1 */
    seq_t *seq = NULL;
    ALLOC_ARRAY(seq_t, n + 1, seq);
    if (!seq) abort();               /* allocation failure */

    /* base case */
    seq[0] = (seq_t)1;

    /* compute successive terms */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t val = 0;

        /* contribution from the numerator */
        if (k <= 41)          val = (seq_t)2;
        else if (k == 42)     val = (seq_t)1;
        /* otherwise val remains 0 for k > 42 */

        /* add 43*a(k-i) for i = 1..min(41,k) */
        idx_t limit = 41;
        if (k < limit) limit = k;
        for (idx_t i = 1; i <= limit; ++i) {
            val += (seq_t)43 * seq[k - i];
        }

        /* subtract 946*a(k-42) when k>=42 */
        if (k >= 42) {
            val -= (seq_t)946 * seq[k - 42];
        }

        seq[k] = val;
    }

    seq_t result = seq[n];

    /* free the array */
    FREE_ARRAY(seq, n + 1);
    return result;
}
