/*
 * A086501  Number of non-commuting permutations: number of ordered pairs g, h
 *          in Symm(n) such that gh <> hg, i.e., the subgroup <g,h> is
 *          non-Abelian.
 * https://oeis.org/A086501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * ( n! - p(n) ) * where p(n) is the number of
 *            partitions of n (OEIS A000041)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */
#include <stdint.h>   /* for uint8_t etc, idx_t and seq_t are provided by the build */

/* Static caches for factorials and partitions. */
static seq_t *fact_cache = NULL;   /* fact_cache[i] = i!   */
static seq_t *part_cache = NULL;   /* parts_cache[i] = p(i) */
static idx_t   cache_size = 0;     /* maximum n that has been cached */

/* Recompute factorials and partition numbers up to n (inclusive). */
static void recompute_up_to(idx_t n)
{
    /* Allocate new arrays. */
    seq_t *new_fact  = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    seq_t *new_part = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    if (!new_fact || !new_part) {
        /* Allocation failed – abort (this should not happen in normal use). */
        exit(1);
    }

    /* Factorial computation. */
    new_fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        new_fact[i] = new_fact[i - 1] * ((seq_t)i);
    }

    /* Partition numbers via Euler's pentagonal recurrence. */
    new_part[0] = (seq_t)1;  /* p(0) = 1 */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum_pos = (seq_t)0;
        seq_t sum_neg = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            /* Generalized pentagonal numbers. */
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            seq_t term = new_part[m - g1];
            if (k & 1)        /* odd k => positive contribution */
                sum_pos += term;
            else
                sum_neg += term;

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > m) continue;
            term = new_part[m - g2];
            if (k & 1)
                sum_pos += term;
            else
                sum_neg += term;
        }
        new_part[m] = sum_pos - sum_neg;
    }

    /* Replace old caches with the new ones. */
    if (fact_cache) free(fact_cache);
    if (part_cache) free(part_cache);
    fact_cache = new_fact;
    part_cache = new_part;
    cache_size = n;
}

/* External sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* out‑of‑range indices produce 0 */

    /* Ensure caches contain values up to n. */
    if (n > cache_size) {
        recompute_up_to(n);
    }

    seq_t fact = fact_cache[n];
    seq_t part = part_cache[n];
    return fact * (fact - part);
}
