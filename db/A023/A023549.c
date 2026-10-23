/*
 * A023549  Convolution of natural numbers >= 2 and Lucas numbers.
 * https://oeis.org/A023549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^{n} (n+2-i) * Lucas(i); Lucas(1)=1, Lucas(2)=3,
 *            Lucas(k)=Lucas(k-1)+Lucas(k-2)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *aarr = NULL;   /* a[1..] */
static seq_t *La  = NULL;   /* Lucas numbers L[1..] */
static idx_t   maxn = 0;    /* highest index computed */
static seq_t   Sprev = 0;   /* sum of Lucas numbers up to index (maxn) */

/* Ensure that aarr[1..n] and La[1..n] are computed */
static void ensure(idx_t n)
{
    if (n <= maxn) return;

    /* Grow arrays to accommodate n (1‑based).  We keep a tiny spare buffer. */
    size_t newcap = (size_t) n + 3;   /* +3 for safety on small indexes */
    aarr = (seq_t *)realloc(aarr, newcap * sizeof(seq_t));
    La  = (seq_t *)realloc(La,  newcap * sizeof(seq_t));

    if (maxn == 0) {
        /* first time: initialise the base values */
        aarr[1] = (seq_t)2;
        La[1]  = (seq_t)1;
        Sprev  = La[1];              /* sum L[1] */
        maxn = 1;
    }

    for (idx_t i = maxn + 1; i <= n; ++i) {
        /* compute Lucas number */
        if (i == 2) {
            La[i] = (seq_t)3;
        } else { /* i >= 3 */
            La[i] = La[i-1] + La[i-2];
        }

        /* compute a(i) using the derived recurrence:
           a(i) = a(i-1) + Sprev + 2*Lucas(i)   where Sprev = sum L[1..i-1] */
        seq_t a_prev = aarr[i-1];
        seq_t Li     = La[i];
        seq_t a_i    = a_prev + Sprev + ((seq_t)2) * Li;
        aarr[i] = a_i;

        /* update the running sum of Lucas numbers */
        Sprev += Li;
    }

    maxn = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n < 1 */
    ensure(n);
    return aarr[(size_t)n];
}
