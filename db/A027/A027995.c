/*
 * A027995  a(n) = Sum_{k=0..2n-1} T(n,k)*T(n,k+1), T given by A027926.
 * https://oeis.org/A027995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..2n-1} T(n,k)*T(n,k+1) where T is A027926
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence derived from the generating function is:  (with a(0)=0, a(1)=2)   */
/* a(n) = 3*a(n-1) + 2*Sum_{k=0..n-2} Catalan(k)*a(n-2-k) + 1 + Sum_{k=0..n-1}Catalan(k) */
  
/* The Catalan numbers are computed as Catalan(k)=binom(2k,k)/(k+1). */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/binom.h"

static seq_t *cat   = NULL;     /* Catalan numbers cat[i] */
static seq_t *pref  = NULL;     /* prefix sums of Catalan numbers pref[i] = sum_{0..i} cat */
static seq_t *seq   = NULL;     /* sequence a[i] */
static size_t  cap  = 0;       /* number of allocated elements */

/* Ensure that arrays are allocated and filled up to index n (inclusive). */
static void ensure_up_to(idx_t n)
{
    if (cap > n) return;          /* already computed */

    size_t new_cap = (size_t)(n + 1);
    cat = (seq_t *)realloc(cat, new_cap * sizeof(seq_t));
    pref = (seq_t *)realloc(pref, new_cap * sizeof(seq_t));
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));

    /* initialise new entries */
    for (idx_t i = (idx_t)cap; i <= n; ++i)
    {
        if (i == 0)                /* Catalan(0)=1 */
        {
            cat[i] = 1;
            pref[i] = 1;
            seq[i] = 0;            /* a(0) not used but defined as 0 */
        }
        else
        {
            /* Catalan(i) = binom(2*i,i)/(i+1) */
            seq_t b = binom(2*i, i);
            cat[i] = b / (seq_t)(i + 1);
            pref[i] = pref[i-1] + cat[i];
            if (i == 1)            /* a(1)=2 */
            {
                seq[i] = 2;
            }
            else
            {
                /* compute sum_{k=0..i-2} cat[k]*a[i-2-k] */
                seq_t s = 0;
                for (idx_t k = 0; k <= i-2; ++k)
                    s += cat[k] * seq[i-2-k];
                /* r = 1 + pref[i-1] */
                seq_t r = 1 + pref[i-1];
                seq[i] = 3 * seq[i-1] + 2 * s + r;
            }
        }
    }
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) /* sequence is defined for n>=1 but we guard */
        return 0;
    ensure_up_to(n);
    return seq[n];
}
