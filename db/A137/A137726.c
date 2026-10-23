/*
 * A137726  Number of sequences of length n with elements {-2,-1,+1,+2},
 *          counted up to simultaneous reversal and negation, such that the
 *          sum of elements of the whole sequence but of no proper subsequence
 *          equals 0 modulo n. For n>=4, the number of Hamiltonian
 *          (undirected) cycles on the circulant graph C_n(1,2).
 * https://oeis.org/A137726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(1)=2, a(2)=2, a(3)=8, a(4)=9, a(5)=12, a(6)=16, a(7)=23,
 *            a(8)=29, a(9)=41 * For n>8: a(n) = a(n-1) + a(n-2) - a(n-5) - 2
 *            * (equivalently a(n) = 2*a(n-1) - a(n-3) - a(n-5) + a(n-6)) *
 *            This recurrence reproduces all known terms
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 58, u64 n <= 116,
 *            u128 n <= 232, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: index 1 corresponds to a(1) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0) return 0; /* undefined, return 0 */

    /* enlarge cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1; /* allocate indices 0 .. n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failed */
        cache = new_cache;
        /* initialize new entries up to new_size-1 */
        for (idx_t i = cache_size; i < new_size; ++i) {
            cache[i] = 0;
        }
        cache_size = new_size;
    }

    /* compute missing terms */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0 || i <= 9) { /* already computed or initial */
            if (i <= 9) {
                /* set base values if not yet set */
                if (cache[i] == 0) {
                    switch (i) {
                        case 1: cache[1] = (seq_t)2; break;
                        case 2: cache[2] = (seq_t)2; break;
                        case 3: cache[3] = (seq_t)8; break;
                        case 4: cache[4] = (seq_t)9; break;
                        case 5: cache[5] = (seq_t)12; break;
                        case 6: cache[6] = (seq_t)16; break;
                        case 7: cache[7] = (seq_t)23; break;
                        case 8: cache[8] = (seq_t)29; break;
                        case 9: cache[9] = (seq_t)41; break;
                    }
                }
            }
            continue;
        }

        /* recurrence for i > 9 */
        if (i > 9) {
            seq_t term = cache[i-1] + cache[i-2] - cache[i-5] - (seq_t)2;
            cache[i] = term;
        }
    }

    return cache[n];
}
