/*
 * A117395  Number of partitions of n into parts that are neither all
 *          squarefree, nor all not squarefree.
 * https://oeis.org/A117395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n) - A073576(n) - A114374(n) computes the number
 *            of partitions of n into parts that are not all squarefree and
 *            not all non‑squarefree
 * Range:     u8 n <= 18, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *partitions   = NULL;
static seq_t *psqfree      = NULL;   /* partitions with all parts squarefree   */
static seq_t *pnonsqfree   = NULL;   /* partitions with all parts non‑squarefree */
static unsigned char *non_sf = NULL;  /* flag array: 1 if number is not squarefree */
static idx_t premax = 0;             /* maximum n precomputed */

/* recompute all values up to limit n */
static void compute_up_to(idx_t n)
{
    if (n <= premax) return;

    /* reallocate arrays to size n+1 */
    partitions = (seq_t*)realloc(partitions,   (n + 1) * sizeof(seq_t));
    psqfree    = (seq_t*)realloc(psqfree,    (n + 1) * sizeof(seq_t));
    pnonsqfree = (seq_t*)realloc(pnonsqfree, (n + 1) * sizeof(seq_t));
    non_sf     = (unsigned char*)realloc(non_sf, (n + 1) * sizeof(unsigned char));

    /* clear arrays */
    for (idx_t i = 0; i <= n; i++) {
        partitions[i] = 0;
        psqfree[i]    = 0;
        pnonsqfree[i] = 0;
        non_sf[i]     = 0;
    }

    /* mark non‑squarefree numbers (numbers divisible by a square >1) */
    for (idx_t i = 2; i * i <= n; i++) {
        idx_t sq = i * i;
        for (idx_t j = sq; j <= n; j += sq) non_sf[j] = 1;
    }

    /* base cases: empty partition */
    partitions[0] = 1;
    psqfree[0]    = 1;
    pnonsqfree[0] = 1;

    /* dynamic programming */
    for (idx_t part = 1; part <= n; part++) {
        if (!non_sf[part]) { /* part is squarefree */
            for (idx_t j = part; j <= n; j++)
                psqfree[j] += psqfree[j - part];
        }
        if (non_sf[part]) { /* part is not squarefree */
            for (idx_t j = part; j <= n; j++)
                pnonsqfree[j] += pnonsqfree[j - part];
        }
        /* partitions with unrestricted parts */
        for (idx_t j = part; j <= n; j++)
            partitions[j] += partitions[j - part];
    }

    premax = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    compute_up_to(n);
    seq_t res = partitions[n];
    res -= psqfree[n];
    res -= pnonsqfree[n];
    return res;
}
