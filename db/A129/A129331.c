/*
 * A129331  Second column of PE^4.
 * https://oeis.org/A129331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * Σ_{k=1}^{n-1} S(n-1,k) * 4^k (S(m,k) are Stirling
 *            numbers of the second kind, S(0,0)=1, S(m,0)=0 for m>0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    if (n == 0)                     /* a(0) = 0 */
        return (seq_t)0;

    /* Cache for B[d] = Σ_{k=1}^{d} S(d,k) * 4^k      */
    static seq_t *B   = NULL;    /* B[d] for d = 0..Blen-1 */
    static size_t Blen = 0;      /* number of computed B entries */

    /* Cache for 4^k   */
    static seq_t *pow4 = NULL;   /* pow4[k] = 4^k, 0..pow4Len-1 */
    static size_t pow4Len = 0;

    /* Cache for current row of Stirling numbers */
    static seq_t *prevS = NULL;  /* prevS[k] = S(d,k) for current d */
    static size_t prevLen = 0;   /* number of entries in prevS (d+1) */

    /* target row (d = n-1) ------------------------------------------------- */
    size_t d = (size_t)(n - 1);

    /* --- initialise cache for d = 0 --------------------------------------- */
    if (Blen == 0) {
        B   = (seq_t *)malloc(sizeof(seq_t));      /* B[0]   = 1          */
        B[0] = (seq_t)1;
        Blen = 1;

        pow4 = (seq_t *)malloc(sizeof(seq_t));     /* pow4[0] = 1          */
        pow4[0] = (seq_t)1;
        pow4Len = 1;

        prevS = (seq_t *)malloc(sizeof(seq_t));   /* S(0,0) = 1           */
        prevS[0] = (seq_t)1;
        prevLen = 1;
    }

    /* --- ensure pow4 has entries up to d --------------------------------- */
    if (pow4Len <= d) {
        size_t newLen = d + 1;
        pow4 = (seq_t *)realloc(pow4, newLen * sizeof(seq_t));
        for (size_t i = pow4Len; i <= d; ++i)
            pow4[i] = pow4[i - 1] * (seq_t)4;
        pow4Len = newLen;
    }

    /* --- grow cache up to d if needed ---------------------------------- */
    if (Blen <= d) {
        /* compute rows S(d,k) for d = Blen .. d */
        for (size_t cur = Blen; cur <= d; ++cur) {
            seq_t *newS = (seq_t *)malloc((cur + 1) * sizeof(seq_t));
            newS[0] = (seq_t)0;                    /* S(cur,0)=0 for cur>0 */

            /* recurrence: S(cur,k) = k*S(cur-1,k)+S(cur-1,k-1) */
            for (size_t k = 1; k <= cur; ++k) {
                seq_t part1 = (k < prevLen) ? prevS[k] * (seq_t)k : (seq_t)0;
                seq_t part2 = (k - 1 < prevLen) ? prevS[k - 1] : (seq_t)0;
                newS[k] = part1 + part2;
            }

            /* compute B[cur] = Σ_{k=1}^{cur} newS[k] * 4^k */
            seq_t Bi = (seq_t)0;
            for (size_t k = 1; k <= cur; ++k)
                Bi += newS[k] * pow4[k];
            if (cur >= Blen) {
                if (cur >= Blen) {
                    B = (seq_t *)realloc(B, (cur + 1) * sizeof(seq_t));
                    B[cur] = Bi;
                }
            }
            /* move to next row */
            free(prevS);
            prevS   = newS;
            prevLen = cur + 1;
        }
        Blen = d + 1;
    }

    /* --- final value ---------------------------------------------------- */
    return ((seq_t)n) * B[d];
}
