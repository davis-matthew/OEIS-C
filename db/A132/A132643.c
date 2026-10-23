/*
 * A132643  Bell number B(n) raised to power B(n).
 * https://oeis.org/A132643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Bell(n-1)^Bell(n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/big_pow.h"

/* static cache for Bell numbers: bell_cache[i] = Bell(i)  with Bell(0)=1 */
static seq_t *bell_cache = NULL;
static size_t bell_cap   = 0;
static idx_t  bell_max   = 0;   /* largest index computed (>=0, with bell_cache[0] initialized) */

/* compute Bell numbers up to index m (inclusive) */
static void compute_bell_upto(idx_t m)
{
    /* grow the cache if necessary */
    if (m + 1 > bell_cap) {
        size_t new_cap = m + 1;
        seq_t *tmp = (seq_t *)realloc(bell_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        bell_cache = tmp;
        bell_cap   = new_cap;
    }

    /* initialise Bell(0) if not done yet */
    if (bell_max == 0) {
        bell_cache[0] = (seq_t)1;   /* Bell(0) = 1 */
    }

    /* compute from bell_max+1 up to m */
    for (idx_t i = bell_max + 1; i <= m; ++i) {
        seq_t sum = (seq_t)0;
        idx_t n_minus_1 = i - 1;
        for (idx_t k = 0; k <= n_minus_1; ++k) {
            seq_t bin = binom(n_minus_1, k);
            seq_t term = bin * bell_cache[k];
            sum += term;
        }
        bell_cache[i] = sum;
    }
    bell_max = m;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset handling */

    idx_t idx = n - 1;                    /* we need Bell(n-1) */
    compute_bell_upto(idx);

    seq_t b = bell_cache[idx];            /* Bell(n-1) */
    return big_pow(b, b);                 /* Bell(n-1)^Bell(n-1) */
}
