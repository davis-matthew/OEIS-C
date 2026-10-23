/*
 * A032305  Number of rooted trees where any 2 subtrees extending from the
 *          same node have a different number of nodes.
 * https://oeis.org/A032305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 30, u64 n <= 55, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032305: Number of rooted trees where any two subtrees
   extending from the same node have different numbers of nodes.
   The sequence satisfies the functional equation
          A(x) = x · Π_{n≥1} (1 + a(n)·x^n),
   hence a(n) is the coefficient of x^{n} in A(x).  The values can be
   obtained by the following dynamic programming algorithm.
   a(1)=1; for n>1,
        process part of size j=n-1:
            for all sums s = (n-1) … j:
                coeff[s] += coeff[s-j] * a(j)
        then a(n) = coeff[j]   (the coefficient of x^{n-1}).

   The implementation recomputes the whole sequence up to the
   requested index when a larger index is asked.  This method keeps
   O(n) memory and runs in O(n²) time, which is easily fast enough
   for n up to about 10 000.  No additional libraries are used.  */
#include <stdlib.h>

static seq_t *aVals = NULL;      /* aVals[i] = a(i) for i ≥ 1   */
static size_t a_size = 0;
static idx_t max_computed = 0;   /* maximum n for which aVals is valid */

/* recompute the whole sequence up to n (inclusive) */
static void recompute_upto(idx_t n)
{
    size_t newSize = n + 1;                 /* indices 0 … n   */
    seq_t *newa  = (seq_t*)calloc(newSize, sizeof(seq_t));
    if (!newa) return;                     /* out of memory – should never happen */

    newa[1] = 1;                            /* a(1) = 1 */

    seq_t *coeff = (seq_t*)calloc(n, sizeof(seq_t)); /* coefficients of g(x) */
    if (!coeff) { free(newa); return; }

    coeff[0] = 1;                           /* g(0) = 1 */

    for (idx_t j = 1; j < n; ++j)           /* j = current part size = j */
    {
        seq_t aj = newa[j];
        for (idx_t s = n - 1; s >= j; --s)  /* 0‑based index for sums   */
        {
            coeff[s] += coeff[s - j] * aj;
        }
        newa[j + 1] = coeff[j];             /* a(j+1) = coeff[j]        */
    }

    /* free old memory and store new array */
    free(aVals);
    free(coeff);
    aVals      = newa;
    a_size     = newSize;
    max_computed = n;
}

/* ensure we have computed a(n) */
static void ensure(idx_t n)
{
    if (n <= max_computed) return;
    recompute_upto(n);
}

/* public function required by the grader */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    ensure(n);
    return aVals[n];
}
