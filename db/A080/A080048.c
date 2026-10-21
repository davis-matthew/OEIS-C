/*
 * A080048  Operation count to create all permutations of n distinct elements
 *          using Algorithm L (lexicographic permutation generation) from
 *          Knuth's The Art of Computer Programming, Vol. 4, chapter 7.2.1.2.
 *          Sequence gives number of loop repetitions in reversal step.
 * https://oeis.org/A080048
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + (n-1)*floor((n+1)/2) for n ≥ 3, a(2) = 1. The
 *            sequence is undefined for n < 2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static idx_t  max_n = 1;       /* highest n that has been computed */
    static seq_t *cache = NULL;    /* dynamic table of a(i) values */

    if (n < 2) return 0;           /* undefined for n < 2 */

    /* allocate cache if needed */
    if (cache == NULL) {
        /* start with room for at most n + 1 values */
        size_t sz = (size_t)(n + 1);
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) return 0;
        max_n = 1;
    }

    /* enlarge cache if necessary */
    if (n > max_n) {
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;

        /* compute missing values */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i == 2) {               /* base case */
                cache[2] = (seq_t)1;
            } else {
                seq_t part1 = (seq_t)i * cache[i - 1];
                seq_t part2 = (seq_t)(i - 1) * ((i + 1) / 2);
                cache[i] = part1 + part2;
            }
        }
        max_n = n;
    }

    return cache[n];
}
