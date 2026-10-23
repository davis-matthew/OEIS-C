/*
 * A160428  Number of ON cells at n-th stage of three-dimensional version of
 *          the cellular automaton A160410, using cubes.
 * https://oeis.org/A160428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8 * Sum_{k=0..n-1} 7^A000120(k) (A000120 = popcount)
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cumsum[i] = Sum_{k=0}^{i-1} 7^popcount(k)  */
    static seq_t *cumsum = NULL;
    static idx_t computed_upto = 0; /* highest index for which cumsum[i] is valid */

    /* initialise the cache for i = 0 */
    if (cumsum == NULL) {
        cumsum = (seq_t *)malloc(sizeof(seq_t));
        if (cumsum == NULL) return (seq_t)0; /* allocation failure -> zero */
        cumsum[0] = (seq_t)0;
        computed_upto = 0;
    }

    /* extend cache up to n if needed */
    if (n > computed_upto) {
        /* allocate space for indices 0..n (cumsum size = n+1) */
        seq_t *tmp = (seq_t *)realloc(cumsum, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure -> zero */
        cumsum = tmp;

        /* compute new entries: for i = computed_upto+1 .. n */
        for (idx_t i = computed_upto + 1; i <= n; ++i) {
            idx_t k = i - 1; /* k is the index whose term is added */
            int pop = __builtin_popcountll((unsigned long long)k);

            seq_t term = (seq_t)1;
            for (int j = 0; j < pop; ++j) {
                term *= (seq_t)7;
            }

            cumsum[i] = cumsum[i - 1] + term;
        }

        computed_upto = n;
    }

    /* return a(n) = 8 * cumsum[n] */
    return (seq_t)8 * cumsum[n];
}
