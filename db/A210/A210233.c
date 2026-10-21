/*
 * A210233  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210234; see the Formula section.
 * https://oeis.org/A210233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened row‑wise coefficient of the polynomials *
 *            u(n,x) defined by * u(1,x)=1, v(1,x)=1, *
 *            u(n,x)=x*u(n‑1,x)+v(n‑1,x)+1, *
 *            v(n,x)=(x+1)*u(n‑1,x)+2x*v(n‑1,x)+1. * a(1)=1 and the terms are
 *            enumerated row by row, i.e. the k‑th row of * u(k,x) has k
 *            coefficients, the first being k and...
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine the row k and offset within the row */
    idx_t low = 1, high = 1, k;

    /* Find an upper bound for k such that k*(k+1)/2 >= n */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;                /* double until large enough */

    /* Binary search for minimal k */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    k = low;

    /* offset inside row (0‑based) */
    unsigned __int128 prev_tri = (unsigned __int128)(k - 1) * k / 2;
    idx_t offset = n - (idx_t)prev_tri - 1;

    /* Allocate arrays of maximum needed size k */
    seq_t *Uprev = (seq_t *)malloc(sizeof(seq_t) * k);
    seq_t *Vprev = (seq_t *)malloc(sizeof(seq_t) * k);
    seq_t *Ucurr = (seq_t *)malloc(sizeof(seq_t) * k);
    seq_t *Vcurr = (seq_t *)malloc(sizeof(seq_t) * k);
    if (!Uprev || !Vprev || !Ucurr || !Vcurr)
    {
        /* Allocation failed – return 0 (should not happen in test harness) */
        free(Uprev); free(Vprev); free(Ucurr); free(Vcurr);
        return (seq_t)0;
    }

    /* Base row */
    Uprev[0] = (seq_t)1;
    Vprev[0] = (seq_t)1;

    /* Build up to row k */
    for (idx_t i = 2; i <= k; ++i)
    {
        /* U[i] */
        Ucurr[0] = Vprev[0] + (seq_t)1;
        for (idx_t j = 1; j <= i - 2; ++j)
            Ucurr[j] = Uprev[j - 1] + Vprev[j];
        Ucurr[i - 1] = Uprev[i - 2];

        /* V[i] */
        Vcurr[0] = Uprev[0] + (seq_t)1;
        for (idx_t j = 1; j <= i - 1; ++j)
        {
            seq_t uprev_j_minus1 = Uprev[j - 1];
            seq_t uprev_j = (j < i - 1) ? Uprev[j] : (seq_t)0;
            seq_t vprev_j_minus1 = Vprev[j - 1];
            Vcurr[j] = uprev_j_minus1 + uprev_j + (seq_t)2 * vprev_j_minus1;
        }

        /* Swap pointers: next iteration will use the newly computed arrays */
        seq_t *tmp = Uprev; Uprev = Ucurr; Ucurr = tmp;
        tmp = Vprev; Vprev = Vcurr; Vcurr = tmp;
    }

    seq_t result = Uprev[offset];

    /* Free memory */
    free(Uprev); free(Vprev); free(Ucurr); free(Vcurr);
    return result;
}
