/*
 * A105266  a(1)=1 and, for n>1, a(n) is the smallest integer greater than
 *          a(n-1) such that no three terms x,y,z of the sequence, with x<y<z,
 *          satisfy z-y=y-x+1.
 * https://oeis.org/A105266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_valid, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A105266
    a(1)=1; for n>1 the smallest integer greater than a(n‑1) that
    makes the sequence avoid triples x<y<z with z‑y = y‑x+1.
    Equivalently, a(n) is the n‑th positive integer whose base‑3
    representation contains at most one digit '1', and if the number
    ends with zeros those zeros are preceded by a '1'.  */

#include <stdbool.h>
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"   /* needed for seq_realloc */
#include "../utils/is_valid.h"

static seq_t *seq   = NULL;          /* dynamic array of terms */
static idx_t  sz    = 0;             /* number of terms stored */
static idx_t  cap   = 0;             /* current allocated capacity */

seq_t a(idx_t n)
{
    if (n == 0) return 0U;           /* undefined for 0, but avoid UB */

    /* Ensure we have at least n terms */
    while (sz < n)
    {
        /* grow capacity when needed */
        if (cap == 0) cap = 16;
        while (cap < n) cap <<= 1;    /* double until sufficient */

        if (seq == NULL)
            seq = seq_malloc(cap);
        else
            seq = seq_realloc(seq, cap);

        /* generate missing terms */
        seq_t cand = (sz == 0) ? 1U : seq[sz-1] + 1U;
        while (sz < n)
        {
            if (is_valid(cand))
                seq[sz++] = cand;
            ++cand;
        }
    }

    return seq[n-1];
}
