/*
 * A227839  3^a(n) is the highest power of 3 dividing A005132(n).
 * https://oeis.org/A227839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3-adic valuation of Recamán's sequence (built
 *            iteratively)
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Computes A227839: a(n) = v_3( Recamán(n) ), where Recamán is defined
   with a(0)=0 and for n>=1: if a(n-1)-n >=0 and not already seen, use it;
   otherwise use a(n-1)+n.  Data are memoised across calls. */

static seq_t *recVals = NULL;   /* Recamán values */
static seq_t *aVals   = NULL;   /* 3-adic valuations */
static size_t cap     = 0;       /* current allocation size */
static idx_t  maxComp = 0;       /* largest n computed so far */

/* Ensure storage up to n and compute Recamán terms up to n */
static void computeUpTo(idx_t n)
{
    if (maxComp >= n) return;

    /* Initial allocation or grow */
    if (recVals == NULL) {
        size_t init = n + 1;
        if (init < 16) init = 16;
        recVals = (seq_t*)malloc(init * sizeof(seq_t));
        aVals   = (seq_t*)malloc(init * sizeof(seq_t));
        cap     = init;
        recVals[0] = (seq_t)0;
        aVals[0]   = (seq_t)0;
    } else if (cap <= (size_t)n) {
        size_t newCap = cap;
        while (newCap <= (size_t)n) newCap <<= 1;
        recVals = (seq_t*)realloc(recVals, newCap * sizeof(seq_t));
        aVals   = (seq_t*)realloc(aVals,   newCap * sizeof(seq_t));
        cap     = newCap;
    }

    for (idx_t k = maxComp + 1; k <= n; ++k) {
        seq_t prev = recVals[k - 1];
        seq_t subVal = (seq_t)0;
        bool canSub = false;

        if (prev >= (seq_t)k) {
            subVal = prev - (seq_t)k;
            /* check if subVal already seen */
            bool seen = false;
            for (idx_t i = 0; i < k - 1; ++i) {
                if (recVals[i] == subVal) { seen = true; break; }
            }
            if (!seen) canSub = true;
        }

        seq_t newVal = canSub ? subVal : prev + (seq_t)k;
        recVals[k] = newVal;

        /* compute v3(newVal) */
        seq_t temp = newVal;
        seq_t cnt  = (seq_t)0;
        while ((temp % (seq_t)3) == (seq_t)0) {
            temp /= (seq_t)3;
            ++cnt;
        }
        aVals[k] = cnt;
    }

    maxComp = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for offset 1, safe return */
    computeUpTo(n);
    return aVals[n];
}
