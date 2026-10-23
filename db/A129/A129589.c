/*
 * A129589  a(n) = 2*A000129(n) + A000129(n-1) - n.
 * https://oeis.org/A129589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000129(n+1) - n (OEIS A129589)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000129 is the Pell sequence: P(0)=0, P(1)=1, P(k)=2*P(k-1)+P(k-2). */

#include <stdlib.h>   /* for malloc, realloc */

static seq_t *pell = NULL;   /* dynamically grown array of Pell numbers */
static size_t pell_cap = 0;   /* current allocation capacity */
static size_t pell_sz  = 0;   /* number of Pell numbers actually stored */

/* Ensure that pell[0] .. pell[k] are computed (k inclusive). */
static void ensure_pell_upto(idx_t k)
{
    /* If we already computed enough, nothing to do. */
    if (pell_sz > (size_t)k)
        return;

    /* Grow the array if necessary. */
    size_t target = (size_t)k + 1;               /* number of elements we need */
    if (pell_cap < target) {
        size_t new_cap = pell_cap ? pell_cap * 2 : 8;
        if (new_cap < target)
            new_cap = target;
        seq_t *tmp = (seq_t *)realloc(pell, new_cap * sizeof(seq_t));
        pell = tmp;
        pell_cap = new_cap;
    }

    /* Initialise the first two Pell numbers if not already done. */
    if (pell_sz == 0) {
        pell[0] = 0;
        if (pell_cap >= 2) {
            pell[1] = 1;
            pell_sz = 2;
        }
    }

    /* Compute missing Pell numbers up to index k. */
    for (size_t i = pell_sz; i <= (size_t)k; ++i)
        pell[i] = 2 * pell[i - 1] + pell[i - 2];

    if (pell_sz < (size_t)k + 1)  /* update stored count */
        pell_sz = (size_t)k + 1;
}

/* Required function: compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)            /* sequence is undefined for n=0; return 0 */
        return 0;

    idx_t k = n + 1;   /* Pell index needed: A000129(n+1) */
    ensure_pell_upto(k);

    return pell[(size_t)k] - (seq_t)n;
}
