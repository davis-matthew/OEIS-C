/*
 * A130632  Number of natural numbers between d(n) and d(n+1), where d(n)
 *          denotes the number of divisors of n.
 * https://oeis.org/A130632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(0, |d(n+1)-d(n)| - 1) where d(k) = number of divisors
 *            of k. This implements the OEIS sequence A130632
 * Range:     u8 n <= 9992, u16 n <= 9992, u32 n <= 9992, u64 n <= 9992,
 *            u128 n <= 9992, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* static cache of divisor counts */
static seq_t *div_counts = NULL;
static idx_t max_computed = 0;

/* Ensure divisor counts up to index `needed` are available */
static void
ensure_div_counts(idx_t needed)
{
    if (needed <= max_computed)
        return;

    /* extend array */
    size_t new_cap = needed + 1;
    div_counts = (seq_t *)realloc(div_counts, new_cap * sizeof(seq_t));
    if (!div_counts) {
        /* allocation failure – abort */
        exit(1);
    }

    /* initialise new entries to zero */
    for (idx_t i = max_computed + 1; i <= needed; ++i)
        div_counts[i] = 0;

    /* update divisor counts for new range */
    for (idx_t i = 1; i <= needed; ++i) {
        /* first multiple of i that is >= max_computed+1 */
        idx_t start = ((max_computed + 1 + i - 1) / i) * i;
        for (idx_t j = start; j <= needed; j += i)
            div_counts[j] += 1;
    }

    max_computed = needed;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    /* we need divisor counts up to n+1 */
    ensure_div_counts(n + 1);

    seq_t dn   = div_counts[n];
    seq_t dnp1 = div_counts[n + 1];
    seq_t diff = (dnp1 > dn) ? (dnp1 - dn) : (dn - dnp1);
    if (diff <= 1)
        return (seq_t)0;
    return diff - 1;
}
