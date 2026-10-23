/*
 * A105479  a(n) = C(n,2)*Bell(n-2) (cf. A000217, A000110).
 * https://oeis.org/A105479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n,2)*Bell(n-2) (OEIS A105479)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static data shared by all calls */
static seq_t *Bell      = NULL;   /* holds Bell[0..Belllen-1] */
static idx_t  Belllen   = 0;      /* number of Bell entries stored   */
static seq_t *prevS     = NULL;   /* previous Stirling row, size = m */
static idx_t  prevSize  = 0;      /* length of prevS                  */
static idx_t  maxComp   = 0;      /* largest m for which Bell[m] is known */

/*------------------------------------------------------------*/
/*  Computes a(n) = C(n,2) * Bell(n-2) for all n >= 0          */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 2)                /* a(0)=a(1)=0 */
        return (seq_t)0;

    /* initialise static storage on first call */
    if (Belllen == 0) {
        Bell   = (seq_t*)malloc(sizeof(seq_t));
        prevS  = (seq_t*)malloc(sizeof(seq_t));
        if (!Bell || !prevS)
            return (seq_t)0;   /* allocation failure */

        Bell[0]      = (seq_t)1;   /* Bell(0) = 1 */
        Belllen      = 1;
        prevS[0]     = (seq_t)1;   /* S(0,0)=1          */
        prevSize     = 1;
        maxComp      = 0;
    }

    /* we must have Bell[n-2] computed */
    idx_t needed = n - 2;
    if (needed > maxComp) {
        for (idx_t m = maxComp + 1; m <= needed; ++m) {
            idx_t rowSize = m + 1;
            seq_t *currS = (seq_t*)malloc(sizeof(seq_t) * rowSize);
            if (!currS)
                return (seq_t)0;   /* allocation failure */

            currS[0] = (seq_t)0;             /* S(m,0) = 0 */
            currS[rowSize-1] = (seq_t)1;     /* S(m,m) = 1 */

            /* compute interior Stirling numbers */
            for (idx_t k = 1; k < m; ++k) {
                seq_t term = (seq_t)k * prevS[k];
                currS[k] = prevS[k-1] + term;
            }

            /* sum row to obtain Bell(m) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < rowSize; ++k)
                sum += currS[k];

            /* store Bell(m) */
            seq_t *tmp = (seq_t*)realloc(Bell, sizeof(seq_t) * (Belllen + 1));
            if (!tmp) {
                free(currS);
                return (seq_t)0;
            }
            Bell = tmp;
            Bell[Belllen] = sum;
            ++Belllen;

            /* prepare for next iteration */
            free(prevS);
            prevS    = currS;
            prevSize = rowSize;
        }
        maxComp = needed;
    }

    /* binomial(n,2) = n*(n-1)/2  (computed in seq_t space) */
    seq_t binom = (seq_t)n * ((seq_t)n - (seq_t)1) / (seq_t)2;

    /* final result */
    return binom * Bell[needed];
}
