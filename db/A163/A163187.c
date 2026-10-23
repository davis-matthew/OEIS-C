/*
 * A163187  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=28, a(2)=756, a(3)=20412;
   for n>=4:  a(n) = 26*a(n-1)+26*a(n-2)+26*a(n-3)-351*a(n-4) + (n==4?1:0) */
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached sequence values */
    static size_t cap = 0;           /* current allocated capacity */
    static idx_t last_calc = 3;      /* highest index that has been computed */

    /* On first invocation, allocate space for the first four terms */
    if (cache == NULL) {
        grow_seq_array(&cache, &cap, 4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)28;
        cache[2] = (seq_t)756;
        cache[3] = (seq_t)20412;
    }

    if (n <= last_calc) {
        return cache[n];
    }

    /* Ensure the cache is large enough for index n */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        grow_seq_array(&cache, &cap, needed);
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = last_calc + 1; i <= n; ++i) {
        seq_t term = (seq_t)26 * cache[i-1] +
                     (seq_t)26 * cache[i-2] +
                     (seq_t)26 * cache[i-3] -
                     (seq_t)351 * cache[i-4];
        if (i == 4) {
            term += (seq_t)1;   /* RHS coefficient for n==4 */
        }
        cache[i] = term;
    }

    last_calc = n;
    return cache[n];
}
