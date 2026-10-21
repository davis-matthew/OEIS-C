/*
 * A061186  Staircase of coefficients of polynomials used for column g.f.s of
 *          triangle A060923.
 * https://oeis.org/A061186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabf
 * Formula:   a(n) is the flattened list of coefficients of the polynomials
 *            pLe(n,x), which satisfy the recurrence p0(x) = 1, p1(x) = 1 + x,
 *            p_n(x) = 2(1 + x) p_{n-1}(x) - (1 - 7x + 13x² - 4x³) p_{n-2}(x),
 *            for n ≥ 2. The sequence a(0),a(1),… is obtained by listing the
 *            coefficients of p0, p1, p2, … in order
 * Range:     i8 n <= 14, i16 n <= 45, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate the first two polynomials p0 and p1 */
    size_t lenPrevPrev = 1;
    seq_t *pPrevPrev = (seq_t *)calloc(lenPrevPrev, sizeof(seq_t));
    if (!pPrevPrev) return 0;
    pPrevPrev[0] = (seq_t)1;            /* p0(x) = 1 */

    size_t lenPrev = 2;
    seq_t *pPrev = (seq_t *)calloc(lenPrev, sizeof(seq_t));
    if (!pPrev) { free(pPrevPrev); return 0; }
    pPrev[0] = (seq_t)1;                /* p1(x) = 1 + x */
    pPrev[1] = (seq_t)1;

    idx_t cur_index = 0;                /* current position in the flattened sequence */

    /* Handle coefficients of p0 */
    for (size_t j = 0; j < lenPrevPrev; ++j)
    {
        if (cur_index == n)
        {
            seq_t ret = pPrevPrev[j];
            free(pPrevPrev); free(pPrev);
            return ret;
        }
        ++cur_index;
    }

    /* Handle coefficients of p1 */
    for (size_t j = 0; j < lenPrev; ++j)
    {
        if (cur_index == n)
        {
            seq_t ret = pPrev[j];
            free(pPrevPrev); free(pPrev);
            return ret;
        }
        ++cur_index;
    }

    /* Iterate for n ≥ 2 */
    idx_t i = 2;
    while (cur_index <= n)
    {
        size_t lenCur = (size_t)i + (size_t)(i / 2) + 1;
        seq_t *pCur = (seq_t *)calloc(lenCur, sizeof(seq_t));
        if (!pCur) { free(pPrevPrev); free(pPrev); return 0; }

        for (size_t j = 0; j < lenCur; ++j)
        {
            seq_t term1 = 0, term2 = 0;

            /* 2*(pPrev[j] + pPrev[j-1]) */
            if (j < lenPrev)          term1 += pPrev[j];
            if (j > 0 && (j - 1) < lenPrev)  term1 += pPrev[j - 1];
            term1 += term1;                   /* multiply by 2 */

            /* (1 - 7x + 13x² - 4x³) * pPrevPrev */
            if (j < lenPrevPrev)          term2 += pPrevPrev[j];
            if (j > 0 && (j - 1) < lenPrevPrev)  term2 -= pPrevPrev[j - 1] * (seq_t)7;
            if (j > 1 && (j - 2) < lenPrevPrev)  term2 += pPrevPrev[j - 2] * (seq_t)13;
            if (j > 2 && (j - 3) < lenPrevPrev)  term2 -= pPrevPrev[j - 3] * (seq_t)4;

            pCur[j] = term1 - term2;

            if (cur_index == n)
            {
                seq_t ret = pCur[j];
                free(pPrevPrev); free(pPrev); free(pCur);
                return ret;
            }
            ++cur_index;
        }

        /* Prepare for next iteration */
        free(pPrevPrev);
        pPrevPrev = pPrev;           /* pPrev becomes the new p_{i-2} */
        lenPrevPrev = lenPrev;

        pPrev = pCur;                /* pCur becomes the new p_{i-1} */
        lenPrev = lenCur;

        ++i;
    }

    /* Should never be reached for valid n */
    return 0;
}
