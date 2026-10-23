/*
 * A191971  Square array A(i,j) related to the square array T(i,j) of A192062.
 * https://oeis.org/A191971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A191971
 *
 * a(n) = A(i,j) where the pair (i,j) is obtained from the Cantor
 *         pairing of n:
 *
 *     k = ⌊(√(8 n + 1) – 1)/2⌋
 *     i = n – k(k + 1)/2
 *     j = k – i
 *
 * The two‑dimensional array A(i,j) is defined by
 *
 *     A(0,j) = 2
 *     A(1,j) = 1
 *     For r > 1:
 *         if r is even (r = 2 p) :  A(r,j)   = j·A(r‑1,j) + A(r‑2,j)
 *         if r is odd  (r = 2 p + 1) : A(r,j) = A(r‑1,j) + A(r‑2,j)
 *
 * The recurrence uses only the current column j, so A(i,j) can be
 * evaluated with a simple scalar loop.
 *
 * The function below implements these rules; it works for any
 * integer type seq_t (including arbitrary‑precision types) and for
 * any idx_t ≥ 0.
 */

#if defined(__cplusplus)
# include <cmath>
#else
# include <math.h>
#endif

seq_t a(idx_t n)
{
    /*---  Cantor pairing: compute i and j from n  ---*/
    /*  8 n + 1 is small for the ranges used in testing  */
    idx_t x = 8 * n + 1;
    long double d = sqrt((double)x);          /* double is sufficient for the
                                               * small arguments used in tests */
    idx_t t = (idx_t)d;                       /* floor(√(8 n+1))       */
    while ((t + 1) * (t + 1) <= x) ++t;       /* adjust for rounding      */
    while (t * t > x) --t;                   /* adjust for rounding      */

    idx_t k  = (t - 1) / 2;
    idx_t t2 = k * (k + 1) / 2;
    idx_t i  = n - t2;
    idx_t j  = k - i;

    /*---  Evaluate A(i,j)  ---*/
    seq_t a0 = (seq_t)2;     /* A(0,j) */
    if (i == 0) return a0;   /* early exit for first row */

    seq_t a1 = (seq_t)1;     /* A(1,j) */
    if (i == 1) return a1;   /* early exit for second row */

    for (idx_t r = 2; r <= i; ++r) {
        seq_t a2;
        if ((r & 1) == 0)            /* even row  r = 2m   */
            a2 = (seq_t)j * a1 + a0; /* A(r,j) = j·A(r-1,j)+A(r-2,j) */
        else                         /* odd  row  r = 2m+1 */
            a2 = a1 + a0;            /* A(r,j) = A(r-1,j)+A(r-2,j)    */
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
