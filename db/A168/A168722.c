/*
 * A168722  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^17 = I.
 * https://oeis.org/A168722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 43 * Σ_{i=0}^{n-1} a(i) + 2 for 1 ≤ n ≤ 16 a(17)
 *            = 43 * Σ_{i=1}^{16} a(i) - 946 * a(0) + 1 a(n) = 43 *
 *            Σ_{i=n-16}^{n-1} a(i) - 946 * a(n-17) for n ≥ 18
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   The routine builds the sequence in a memoised array.  The array
 *   grows only when a larger index n is requested, so each term is
 *   computed at most once.  All arithmetic is performed in seq_t,
 *   which may be a builtin integer type or an arbitrary‑precision
 *   integer class.  no floating point or pow() is used. */

#include <stdlib.h>   /* for realloc, abort */
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *tab = NULL;
    static size_t cache_size = 0;

    /* if we already have the value, return it immediately */
    if ((size_t)n < cache_size)
        return tab[n];

    /* reallocate to hold up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(tab, new_size * sizeof(seq_t));
    if (!tmp && new_size)  /* allocation failure */
        abort();
    tab = tmp;

    /* compute values from the current cache_size up to n */
    for (idx_t k = (idx_t)cache_size; k <= n; ++k) {
        if (k == 0) {
            tab[0] = (seq_t)1;
            continue;
        }

        if (k <= 16) {          /* 1 ≤ k ≤ 16 */
            seq_t sum = 0;
            for (idx_t i = 0; i < k; ++i)
                sum += tab[i];
            tab[k] = (seq_t)43 * sum + (seq_t)2;
            continue;
        }

        if (k == 17) {          /* k = 17 */
            seq_t sum = 0;
            for (idx_t i = 1; i <= 16; ++i)
                sum += tab[i];
            tab[17] = (seq_t)43 * sum - (seq_t)946 * tab[0] + (seq_t)1;
            continue;
        }

        /* k ≥ 18 */
        seq_t sum = 0;
        for (idx_t i = k - 16; i < k; ++i)
            sum += tab[i];
        tab[k] = (seq_t)43 * sum - (seq_t)946 * tab[k - 17];
    }

    /* update the size of the stored prefix */
    cache_size = new_size;
    return tab[n];
}
