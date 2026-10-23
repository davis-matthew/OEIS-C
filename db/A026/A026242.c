/*
 * A026242  a(n) = j if n is L(j), else a(n) = k if n is U(k), where L =
 *          A000201, U = A001950 (lower and upper Wythoff sequences).
 * https://oeis.org/A026242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = the index of n in the Wythoff lower or upper sequence
 *            (lower = floor(k*φ), upper = floor(k*φ²) = lower + k). The value
 *            is the smallest k such that n = floor(k*φ) or n = floor(k*φ²).
 *            This implementation generates the sequences incrementally using
 *            the complementary property; each integer app...
 * Range:     u8 n <= 413, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are defined by the build system. */
#include <stdlib.h>
#include <stdbool.h>

/* static cache variables */
static seq_t *idx_of = NULL;          /* mapping from number to its index */
static unsigned char *used = NULL;    /* marker of numbers already used */
static idx_t max_n = 0;               /* highest n for which idx_of is valid */

/* Forward declaration */
static void recompute_upto(idx_t limit);

/* Public function required by the problem statement */
seq_t a(idx_t n)
{
    /* Expand cache if necessary */
    if (n > max_n) {
        recompute_upto(n);
    }
    return idx_of[n];
}

/* Helper that recomputes the cache up to 'limit' (n).  It allocates
   new arrays and fills them with the mapping from each integer 1..limit
   to the index of its Wythoff pair.  Complexity O(limit). */
static void recompute_upto(idx_t limit)
{
    /* Allocate new arrays */
    size_t sz = (size_t)limit + 1;          /* +1 to allow 1-based indexing */
    free(idx_of);
    free(used);
    idx_of = (seq_t *)calloc(sz, sizeof(seq_t));
    used   = (unsigned char *)calloc(sz, sizeof(unsigned char));

    /* Variables for sequence generation */
    idx_t last_a = 0;          /* last lower Wythoff number generated */
    idx_t assigned = 0;        /* count of numbers <= limit already assigned */

    for (idx_t i = 1; ; ++i) {
        /* Find the next unused integer greater than last_a */
        idx_t cand = last_a + 1;
        while (cand <= limit && used[cand]) {
            ++cand;
        }
        /* If no unused candidate remains, all numbers have been assigned */
        if (cand > limit) {
            break;
        }
        last_a = cand;
        idx_t b = cand + i;     /* upper corresponds to lower + i */

        /* Map the lower number */
        idx_of[cand] = (seq_t)i;
        used[cand] = 1;
        ++assigned;

        /* Map the upper number if it lies within the needed range */
        if (b <= limit) {
            idx_of[b] = (seq_t)i;
            used[b] = 1;
            ++assigned;
        }

        /* All numbers up to 'limit' have been assigned? */
        if (assigned >= limit) {
            break;
        }
    }

    max_n = limit;
}
