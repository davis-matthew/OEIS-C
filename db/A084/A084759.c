/*
 * A084759  Composite numbers in ascending order such that the difference of
 *          successive terms is unique. a(m) - a(m-1) = a(k) - a(k-1) iff m =
 *          k.
 * https://oeis.org/A084759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   isComposite, isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A084759
   a(1) = 4
   a(n) = the smallest composite integer > a(n-1) whose difference
          a(n) - a(n-1) did not occur as a previous difference.
   Differences are stored to enforce uniqueness. */

/*======================================================================*/
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isComposite.h"
#include "../utils/isPrimeTrial.h"

/*--------------------------------------------------------------------*/
/*  Static storage for the sequence and for the set of used differences */
static seq_t *terms      = NULL;   /* a(1), a(2), ...            */
static size_t termsSize  = 0;      /* number of stored terms      */
static size_t termsCap   = 0;      /* capacity of Terms array     */

static bool *diffUsed   = NULL;    /* diffUsed[d] == true if d was seen */
static size_t diffCap   = 0;       /* capacity of diffUsed array    */

/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*  Grow the terms array so that it has at least minCap elements.     */
static inline void grow_terms(size_t minCap)
{
    if (termsCap >= minCap) return;
    size_t newCap = (termsCap == 0) ? 4 : termsCap * 2;
    while (newCap < minCap) newCap <<= 1;
    terms = (seq_t*)realloc(terms, newCap * sizeof(seq_t));
    if (!terms) abort();               /* Omitted: real code would handle error */
    termsCap = newCap;
}

/*--------------------------------------------------------------------*/
/*  Grow the diffUsed array to hold indices up to minCap‑1.           */
static inline void grow_diffUsed(size_t minCap)
{
    if (diffCap >= minCap) return;
    size_t newCap = (diffCap == 0) ? 8 : diffCap * 2;
    while (newCap < minCap) newCap <<= 1;
    diffUsed = (bool*)realloc(diffUsed, newCap * sizeof(bool));
    if (!diffUsed) abort();
    for (size_t i = diffCap; i < newCap; ++i) diffUsed[i] = false;
    diffCap = newCap;
}

/*--------------------------------------------------------------------*/
/*  Compute all terms up to index n (1‑based). */
static inline void compute_until(idx_t n)
{
    if (termsSize >= (size_t)n) return;   /* already computed */

    /* Initialise first term if necessary. */
    if (termsSize == 0) {
        grow_terms(2);                 /* space for a(1) and potentially a(2) */
        terms[0] = (seq_t)4;           /* a(1) */
        termsSize = 1;
    }

    seq_t last = terms[termsSize - 1];

    /* Generate terms until we reach the requested index. */
    while (termsSize < (size_t)n) {
        seq_t cand = last + 1;          /* start checking from the next integer */

        for (;;) {
            /* Only composite numbers are candidates. */
            if (!isComposite(cand)) { cand++; continue; }

            seq_t diff = cand - last;
            idx_t idxDiff = (idx_t)diff;      /* conversion to native width */
            size_t diffIdx = (size_t)idxDiff; /* index into diffUsed */

            grow_diffUsed(diffIdx + 1);

            if (!diffUsed[diffIdx]) {          /* unique difference found */
                diffUsed[diffIdx] = true;

                grow_terms(termsSize + 1);
                terms[termsSize] = cand;
                termsSize++;
                last = cand;
                break;                         /* go on to next term */
            }
            cand++;                               /* try next integer */
        }
    }
}

/*--------------------------------------------------------------------*/
/*  The required public function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, harmless */
    compute_until(n);
    return terms[n - 1];
}
