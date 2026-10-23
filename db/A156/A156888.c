/*
 * A156888  Square array T(n, k) = Product_{j=1..n} ( Sum_{i=0..j-1} ( (k+1)^7
 *          - Sum_{m=1..5} (k+1)^m )^i ) with T(n, 0) = n!, read by
 *          antidiagonals.
 * https://oeis.org/A156888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) depends on the square array T(r,k) read by antidiagonals
 *            where T(r,k) = ∏_{j=1..r} ( 1 + f + f² + … + f^{\,j-1} ) ,
 *            T(0,k)=1 , f = ((k+1)/k) * ( (k+1)^7 – (k+1)^6 – (k+1)^5 + 1 ),
 *            k>0 and for k = 0, T(r,0) = r! . The index n is mapped to (r,k)
 *            by: let s be the unique integer with s*(s+1)/2 ≤...
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 17, u64 n <= 24, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Factorial for k = 0 case.  Works for all seq_t sizes. */
static seq_t factorial(idx_t m)
{
    seq_t res = 1;
    for (idx_t i = 2; i <= m; ++i)
        res *= (seq_t)i;
    return res;
}

/* Compute T(n,k) as defined above.  Handles n = 0 and k = 0. */
static seq_t compute_T(idx_t n, idx_t k)
{
    if (n == 0)
        return (seq_t)1;          /* product over empty set is 1 */

    if (k == 0)
        return factorial(n);

    /* Compute f = ((k+1)/k)*( (k+1)^7 – (k+1)^6 – (k+1)^5 + 1 )   */
    seq_t B = (seq_t)(k + 1);        /* B = k+1 */
    seq_t p[8];                      /* p[e] = B^e,  e = 0..7  */
    p[0] = (seq_t)1;
    for (int e = 1; e <= 7; ++e)
        p[e] = p[e - 1] * B;

    seq_t A = p[7] - p[6] - p[5] + (seq_t)1; /* (k+1)^7 – (k+1)^6 – (k+1)^5 + 1 */
    seq_t Ak = A / (seq_t)k;                  /* A/k is exact by definition   */
    seq_t f  = A + Ak;                         /* final f value                */

    /* Compute product ∏_{j=1..n} (1 + f + f² + … + f^{j-1}) */
    seq_t power = (seq_t)1;   /* f^{j-1} during iteration, starts with f⁰ = 1   */
    seq_t sum   = (seq_t)1;   /* current sum = 1 + f + … + f^{j-1}              */
    seq_t result = (seq_t)1;  /* final product                              */

    for (idx_t j = 1; j <= n; ++j) {
        result *= sum;        /* multiply by current sum                    */
        power *= f;           /* advance to next power f^j                  */
        sum   += power;       /* update running sum for next iteration      */
    }
    return result;
}

/*  Public interface required by the framework. */
seq_t a(idx_t n)
{
    /* Find s such that s*(s+1)/2 ≤ n < (s+1)*(s+2)/2. */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;

    idx_t start = s * (s + 1) / 2;
    idx_t r = n - start;          /* row offset   */
    idx_t k = s - r;              /* column index */

    return compute_T(r, k);
}
