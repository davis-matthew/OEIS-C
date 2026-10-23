/*
 * A170561  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence for A170561:
   For n >= 0,
     a(n) = coeff(n) + 22 * Σ_{i=1}^{min(46,n)} a(n-i)   – 253 * [n≥47] * a(n-47)
   where 
     coeff(0) = 1, coeff(1) … coeff(46) = 2, coeff(47) = 1, and coeff(n)=0 for n>47.
   This follows from the rational generating function 
     (t^47 + 2t^46 + … + 2t + 1) / (253t^47 – 22t^46 – … – 22t + 1).
   The sequence is computed on demand and cached.
*/

#include <stddef.h>
#include <stdlib.h>

/* User supplied integer types:
   - idx_t   : unsigned integer type for indices (uint64_t)
   - seq_t   : unsigned integer type (8‑128 bits or arbitrary‑precision)   */

seq_t a(idx_t n)
{
    /* Static cache */
    static seq_t *cache = NULL;   /* dynamic array of seq_t */
    static size_t cap   = 0;      /* allocated capacity */
    static idx_t last   = (idx_t)-1; /* last index computed */

    /* Allocate initial storage if necessary */
    if (!cache) {
        cap   = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure */
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        last = 0;
    }

    /* If requested index is already known, return it */
    if (n <= last) {
        return cache[(size_t)n];
    }

    /* Resize cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t coeff = 0;
        if (i == 0) {
            coeff = (seq_t)1;
        } else if (i <= 46) {
            coeff = (seq_t)2;
        } else if (i == 47) {
            coeff = (seq_t)1;
        } else {
            coeff = (seq_t)0;  /* for i > 47 */
        }

        seq_t term = coeff;

        /* Add 22 * sum of previous up to 46 terms */
        for (idx_t j = 1; j <= 46 && i >= j; ++j) {
            term = term + (cache[(size_t)(i - j)] * (seq_t)22);
        }

        /* Subtract 253 * a(i-47) for i >= 47 */
        if (i >= 47) {
            term = term - (cache[(size_t)(i - 47)] * (seq_t)253);
        }

        cache[(size_t)i] = term;
    }

    last = n;
    return cache[(size_t)n];
}
