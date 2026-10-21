/*
 * A086864  a(n) = (n-1)*(n-2)*(n-3)*(3*n-10)*3^(n-5)/4.
 * https://oeis.org/A086864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A086864
   Formula: a(n) = (n-1)*(n-2)*(n-3)*(3*n-10)*3^n / 972
   Derived from a(n) = (n-1)*(n-2)*(n-3)*(3*n-10)*3^(n-5)/4
   where 3^(n-5) = 3^n / 3^5 = 3^n / 243, and 243*4 = 972.
   Handles all n ≥ 1 (offset 1).
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Use a cache for powers of 3: pow3_cache[i] = 3^i */
    static seq_t *pow3_cache = NULL;
    static idx_t pow3_cache_size = 0;       /* current cache size, i.e. next free index */

    /* Ensure cache covers exponent n */
    if (pow3_cache_size <= n) {
        size_t new_size = (size_t)n + 1;   /* need indices 0..n */
        seq_t *tmp = (seq_t *)realloc(pow3_cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failure – return 0 as fallback */
        pow3_cache = tmp;

        /* initialise beginning if cache was empty */
        if (pow3_cache_size == 0) {
            pow3_cache[0] = (seq_t)1;
            pow3_cache_size = 1;
        }

        /* fill missing powers */
        for (idx_t i = pow3_cache_size; i <= n; ++i) {
            pow3_cache[i] = pow3_cache[i - 1] * (seq_t)3;
        }
        pow3_cache_size = new_size;
    }

    /* Compute the product part */
    seq_t n1   = (seq_t)(n - 1);
    seq_t n2   = (seq_t)(n - 2);
    seq_t n3   = (seq_t)(n - 3);
    seq_t factor = (seq_t)3 * (seq_t)n - (seq_t)10;

    seq_t prod = n1;
    prod *= n2;
    prod *= n3;
    prod *= factor;
    prod *= pow3_cache[n];             /* 3^n */

    /* Divide by 972 (243*4) */
    seq_t result = prod / (seq_t)972;
    return result;
}
