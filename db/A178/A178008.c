/*
 * A178008  Number of permutations of 1..n with no element
 *          e[i>=2]<e[1+floor((i-2)/3)] (3-way heap).
 * https://oeis.org/A178008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(n-1, s1) * binom(n-1-s1, s2) * a(s1) * a(s2) *
 *            a(s3) * where s1, s2, s3 are the sizes of the sub‑trees rooted
 *            at nodes 2,3,4 in * a complete 3‑way heap of n nodes. Base
 *            cases: a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 40,
 *            big any n
 * Helpers:   subtree_size
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/subtree_size.h"

/* --- Internal helpers --------------------------------------------------- */
/* Multiplicative binomial coefficient: binom(n,k) for idx_t indices. */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;               /* use the smaller side */
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        seq_t numerator = (seq_t)(n - k + i);
        res = (res * numerator) / (seq_t)i; /* exact division */
    }
    return res;
}

/* Memoized array of already computed a(n) values. */
static seq_t *a_cache   = NULL;  /* values */
static unsigned char *a_seen = NULL;  /* 0 = unknown, 1 = known */
static size_t a_cap = 0;

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Base cases: a(0)=a(1)=a(2)=1 */
    if (n <= (idx_t)2) return (seq_t)1;

    /* Grow cache if necessary. */
    if ((size_t)n >= a_cap) {
        size_t new_cap = n + 1;
        if (a_cap == 0) {
            a_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            a_seen  = (unsigned char *)malloc(new_cap * sizeof(unsigned char));
            if (!a_cache || !a_seen) { /* allocation failure */
                return (seq_t)0;
            }
        } else {
            a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
            a_seen  = (unsigned char *)realloc(a_seen,  new_cap * sizeof(unsigned char));
            if (!a_cache || !a_seen) {
                return (seq_t)0;
            }
        }
        /* initialise new slots to 'unknown' */
        for (size_t i = a_cap; i < new_cap; ++i) a_seen[i] = 0;
        a_cap = new_cap;
    }

    if (a_seen[n]) return a_cache[n];

    /* Sub‑tree sizes for the three children of the root. */
    idx_t s1 = subtree_size((idx_t)2, n);
    idx_t s2 = subtree_size((idx_t)3, n);
    idx_t s3 = subtree_size((idx_t)4, n);

    /* Compute the multinomial coefficient and multiply by
       the numbers of heap‑valid permutations of each sub‑tree. */
    seq_t res = binom_idx(n - (idx_t)1, s1);
    res = res * binom_idx((n - (idx_t)1) - s1, s2);
    res = res * a(s1);
    res = res * a(s2);
    res = res * a(s3);

    a_cache[n] = res;
    a_seen[n]  = 1;
    return res;
}
