/*
 * A184897  a(n) = (8^n/n!^2) * Product_{k=0..n-1} (16k+1)*(16k+7).
 * https://oeis.org/A184897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8^n/n!^2) * Product_{k=0..n-1} (16k+1)(16k+7) a(0) = 1
 *            a(n+1) = a(n) * 8 * (16n+1)(16n+7) / (n+1)^2 (recurrent
 *            definition used for efficient computation)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

/* cache for values of a(n); a_cache[i] holds a(i) for 0 <= i < a_cache_cap */
static seq_t *a_cache = NULL;   /* dynamic array of computed a(i) */
static size_t  a_cache_cap = 0; /* first free index (size of array) */

/* compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* initialisation: a(0) = 1 */
    if (a_cache == NULL) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!a_cache) abort();           /* allocation failure */
        a_cache[0] = (seq_t)1;           /* a(0) */
        a_cache_cap = 1;
    }

    /* if needed, grow the cache up to n+1 entries */
    if ((size_t)n >= a_cache_cap) {
        size_t new_cap = a_cache_cap ? a_cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failure */
        a_cache = tmp;

        /* compute all missing terms up to a(n) */
        for (idx_t i = a_cache_cap; i <= n; ++i) {
            /* index of previous term, i-1 >= 0 for i >= 1 */
            seq_t prev = a_cache[i - 1];
            idx_t k = i - 1; /* current n in recurrence */

            /* numerator factors */
            seq_t num1 = (seq_t)8;
            seq_t num2 = (seq_t)16 * (seq_t)k + (seq_t)1;
            seq_t num3 = (seq_t)16 * (seq_t)k + (seq_t)7;

            /* denominator factors (i^2 = i * i) */
            seq_t den1 = (seq_t)i;
            seq_t den2 = (seq_t)i;

            /* reduce fractions to keep intermediate values small
               (only works while gcd is efficient for seq_t) */
            seq_t g;

            g = gcd(num1, den1);  
            if (g > (seq_t)1) { num1 /= g; den1 /= g; }
            g = gcd(num1, den2);  
            if (g > (seq_t)1) { num1 /= g; den2 /= g; }

            g = gcd(num2, den1);  
            if (g > (seq_t)1) { num2 /= g; den1 /= g; }
            g = gcd(num2, den2);  
            if (g > (seq_t)1) { num2 /= g; den2 /= g; }

            g = gcd(num3, den1);  
            if (g > (seq_t)1) { num3 /= g; den1 /= g; }
            g = gcd(num3, den2);  
            if (g > (seq_t)1) { num3 /= g; den2 /= g; }

            /* multiply and divide */
            seq_t res = prev;
            res *= num1;
            res *= num2;
            res *= num3;
            res /= den1;
            res /= den2;

            a_cache[i] = res;
        }

        a_cache_cap = n + 1;
    }

    return a_cache[n];
}
