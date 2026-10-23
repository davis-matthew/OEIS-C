/*
 * A141307  Number of bicolored connected permutations.
 * https://oeis.org/A141307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * c(n), where c(1)=1 and for n>1 c(n) = n! –
 *            Σ_{k=1}^{n-1} c(k) * (n-k)! (recurrence for connected
 *            permutations)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *fact   = NULL;
    static seq_t *pow2   = NULL;
    static seq_t *c_cache= NULL;
    static seq_t *a_cache= NULL;
    static size_t cap    = 0;
    static idx_t last    = 0;          /* largest n for which values are computed */

    /* Allocate or grow storage if needed */
    if (fact == NULL) {
        cap = (size_t)n + 1;
        fact   = (seq_t *)malloc(cap * sizeof(seq_t));
        pow2   = (seq_t *)malloc(cap * sizeof(seq_t));
        c_cache= (seq_t *)malloc(cap * sizeof(seq_t));
        a_cache= (seq_t *)malloc(cap * sizeof(seq_t));
        /* base definitions */
        fact[0]    = (seq_t)1;
        pow2[0]    = (seq_t)1;
        c_cache[0] = (seq_t)0;
        a_cache[0] = (seq_t)0;
        last = 0;
    }
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        fact   = (seq_t *)realloc(fact,   new_cap * sizeof(seq_t));
        pow2   = (seq_t *)realloc(pow2,   new_cap * sizeof(seq_t));
        c_cache= (seq_t *)realloc(c_cache,new_cap * sizeof(seq_t));
        a_cache= (seq_t *)realloc(a_cache,new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute factorials and powers of two up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        fact[i] = fact[i-1] * (seq_t)i;
        pow2[i] = pow2[i-1] * (seq_t)2;
    }

    /* Compute c(n) and a(n) for i = last+1 .. n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1) {
            c_cache[i] = (seq_t)1;
            a_cache[i] = pow2[i] * c_cache[i];
        } else {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k < i; ++k) {
                sum += c_cache[k] * fact[i - k];
            }
            c_cache[i] = fact[i] - sum;
            a_cache[i] = pow2[i] * c_cache[i];
        }
    }

    last = n;
    return a_cache[n];
}
