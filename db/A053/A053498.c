/*
 * A053498  Number of degree-n permutations of order dividing 8.
 * https://oeis.org/A053498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations of n elements whose cycle lengths
 *            are in {1,2,4,8}. Computed with the recurrence a(0)=1 and for
 *            n>0 a(n) = Σ_{k∈{1,2,4,8}, k≤n} a(n−k) * Π_{i=n−k+1}^{n−1} i
 *            (empty product = 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>   /* realloc, malloc, free */

seq_t a(idx_t n)
{
    /* static cache so calls in arbitrary order are fast */
    static seq_t *f = NULL;
    static size_t fsize = 0;

    /* enlarge cache if needed, keeping zeros for new entries */
    if (fsize <= n) {
        size_t newsize = n + 1;
        seq_t *tmp = (seq_t *)realloc(f, newsize * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – return 0 as safe fallback */
            return 0;
        }
        f = tmp;
        for (size_t i = fsize; i < newsize; ++i)
            f[i] = 0;
        fsize = newsize;
    }

    /* initialise base case */
    if (f[0] == 0)
        f[0] = 1;

    /* compute all values up to n that are still zero */
    for (idx_t i = 1; i <= n; ++i) {
        if (f[i] != 0) continue;   /* already computed */

        seq_t val = 0;
        /* allowed cycle lengths */
        const idx_t kc[4] = {1, 2, 4, 8};
        for (unsigned t = 0; t < 4; ++t) {
            idx_t k = kc[t];
            if (k > i) break;

            seq_t prod = 1;
            /* product from i-k+1 to i-1 (empty if k==1) */
            for (idx_t j = i - k + 1; j <= i - 1; ++j)
                prod *= (seq_t)j;

            val += prod * f[i - k];
        }
        f[i] = val;
    }
    return f[n];
}
