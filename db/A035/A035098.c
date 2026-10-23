/*
 * A035098  Near-Bell numbers: partitions of an n-multiset with multiplicities
 *          1, 1, 1, ..., 1, 2.
 * https://oeis.org/A035098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 1; a(n) = (Bell(n-2) + Bell(n-1) + Bell(n)) / 2
 *            for n >= 2 (Bell(0) = 1, Bell(1) = 1, Bell(2) = 2, …)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    /* dynamic cache of Bell numbers, indexed from 0 */
    static seq_t *bell = NULL;          /* bell[i] = Bell(i) */
    static idx_t max_computed = 0;      /* highest i for which bell[i] is valid */

    /* expand cache up to n if necessary */
    if (n > max_computed) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(bell, new_cap * sizeof(seq_t));
        bell = tmp;

        /* first time, initialise Bell(0) */
        if (max_computed == (idx_t)0)
            bell[0] = (seq_t)1;

        /* compute Bell numbers from (max_computed+1) up to n */
        for (idx_t cur = max_computed + 1; cur <= n; ++cur) {
            seq_t sum   = 0;
            seq_t binom = 1;       /* binom(cur-1,0) */
            idx_t m = cur - 1;

            for (idx_t k = 0; k <= m; ++k) {
                sum += binom * bell[k];
                if (k < m) {
                    /* next binomial coefficient */
                    binom = binom * (seq_t)(m - k);
                    binom = binom / (seq_t)(k + 1);
                }
            }
            bell[cur] = sum;
        }
        max_computed = n;
    }

    /* a(n) = (Bell(n-2) + Bell(n-1) + Bell(n)) / 2 for n >= 2 */
    return (bell[n-2] + bell[n-1] + bell[n]) / (seq_t)2;
}
