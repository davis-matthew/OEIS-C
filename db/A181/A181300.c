/*
 * A181300  Number of columns with top entry equal to bottom entry in all the
 *          2-compositions of n. A 2-composition of n is a nonnegative matrix
 *          with two rows, such that each column has at least one nonzero
 *          entry and whose entries sum up to n.
 * https://oeis.org/A181300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function
   A(z) = z^2(1‑z)^3 / [(1+z)(1‑4z+2z^2)^2].
   The coefficients a(n) satisfy
     a(n) = 7 a(n‑1) – 12 a(n‑2) – 4 a(n‑3) + 12 a(n‑4) – 4 a(n‑5) + c(n),
   with
     a(0) = 0, a(1) = 0,
     c(2) = 1,  c(3) = –3,  c(4) = 3,  c(5) = –1,  c(n) = 0 for n ≥ 6.
   This recurrence allows computation of a(n) in O(n) time.  A static cache
   is used so that repeated calls with arbitrary n values are efficient. */

#include <stdlib.h>

static seq_t *cache = NULL;     /* array of computed values */
static size_t  cap   = 0;       /* allocated size of cache   */
static idx_t   last  = (idx_t)-1; /* highest n that has been computed */

seq_t a(idx_t n)
{
    /* initial allocation if necessary */
    if (cache == NULL) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();          /* failure: stop */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        last = 1;
    }

    /* grow cache to at least n+1 entries */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap;
        while ((size_t)n + 1 > newcap) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cap = newcap;
    }

    /* compute values up to n if not already done */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = (seq_t)0;

        if (i >= 1) val += ((seq_t)7)  * cache[i-1];
        if (i >= 2) val -= ((seq_t)12) * cache[i-2];
        if (i >= 3) val -= ((seq_t)4)  * cache[i-3];
        if (i >= 4) val += ((seq_t)12) * cache[i-4];
        if (i >= 5) val -= ((seq_t)4)  * cache[i-5];

        /* add the inhomogeneous part c(i) */
        if (i == 2)          val += ((seq_t)1);
        else if (i == 3)     val -= ((seq_t)3);
        else if (i == 4)     val += ((seq_t)3);
        else if (i == 5)     val -= ((seq_t)1);
        /* else c(i)=0 for i>=6 */

        cache[i] = val;
    }
    last = n;

    return cache[n];
}
