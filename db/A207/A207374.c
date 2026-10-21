/*
 * A207374  Composites of the form 24n - 1.
 * https://oeis.org/A207374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th composite number of the form 24 k – 1
 * Range:     u8 n <= 4, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

static seq_t *composites = NULL;   /* array of found composites          */
static size_t  comp_capacity = 0;   /* allocated array capacity           */
static size_t  comp_count    = 0;   /* number of composites stored        */
static idx_t   next_k        = 1;   /* next candidate index k (k ≥ 1)     */

/* Ensure that at least 'need' composites are stored in 'composites'. */
static void ensure_composites(size_t need)
{
    if (need <= comp_count)
        return;                       /* already enough */

    /* Allocate or grow the array if necessary. */
    if (composites == NULL) {
        comp_capacity = need + 10;
        composites = (seq_t *)malloc(comp_capacity * sizeof(seq_t));
    } else if (need > comp_capacity) {
        while (comp_capacity < need)
            comp_capacity <<= 1;     /* double until large enough */
        composites = (seq_t *)realloc(composites, comp_capacity * sizeof(seq_t));
    }

    /* Generate composites until we have 'need' of them. */
    while (comp_count < need) {
        seq_t val = (seq_t)next_k * (seq_t)24 - 1ULL; /* 24*k - 1  */
        ++next_k;
        if (!isPrimeGeneric(val) && val > 1) {        /* composite check */
            composites[comp_count++] = val;
        }
    }
}

/* Return the n-th term of sequence A207374.  The sequence is 1‑based. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for n < 1 */

    ensure_composites((size_t)n);
    return composites[n - 1];
}
