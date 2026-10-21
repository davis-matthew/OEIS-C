/*
 * A236223  Sum of the fifteenth powers of the first n primes.
 * https://oeis.org/A236223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..n} prime(k)^15
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 8, u128 n <= 68,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* static cache of prefix sums: pref[0] = 0,
       pref[i] = sum_{k=1..i} prime(k)^15  (i>=1)   */
    static seq_t *pref = NULL;
    static idx_t pref_len = 0;   /* last computed index */
    static idx_t pref_cap = 0;   /* allocated capacity (number of elements) */

    /* Initial allocation */
    if (!pref)
    {
        pref_cap = (n > 16 ? n + 4 : 17);   /* at least 17 elements */
        pref = (seq_t*)malloc(pref_cap * sizeof(seq_t));
        if (!pref) return 0;                 /* out of memory */
        pref[0] = 0;
        pref_len = 0;
    }

    /* Grow capacity if needed (ensure index n is valid) */
    if (n >= pref_cap)
    {
        idx_t new_cap = n + 4;
        seq_t *tmp = (seq_t*)realloc(pref, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        pref = tmp;
        pref_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = pref_len + 1; i <= n; ++i)
    {
        uint32_t p = get_prime(i);                /* prime(i) */
        seq_t term = big_pow((seq_t)p, (seq_t)15);/* p^15 */
        pref[i] = pref[i-1] + term;               /* prefix sum */
    }
    pref_len = n;

    return pref[n];
}
