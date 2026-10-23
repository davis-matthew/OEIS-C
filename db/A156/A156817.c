/*
 * A156817  A triangle sequence from polynomial coefficients: p(x, n) = (4*(n
 *          - 1) + (4(n - 1))*x^2)*p(x, n - 1) + 2*(n - 1)x^2*p(x, n - 2).
 * https://oeis.org/A156817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^k in the polynomial p(x,r), where r
 *            = ⌊√n⌋ is the row index. The polynomials satisfy p(x,0)=1,
 *            p(x,1)=1+2x+x²,
 *            p(x,n)=(4(n-1)+4(n-1)x²)p(x,n-1)+2(n-1)x²p(x,n-2) for n≥2. The
 *            sequence lists all triangle coefficients row‑wise
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* compute integer square root of n (row index) */
    idx_t r = (idx_t)floor(sqrt((long double)n));
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    idx_t k = n - r * r;           /* index within the row   */

    /* base rows */
    if (r == 0) return (seq_t)1;
    if (r == 1) {
        /* row 1 : {1,2,1} */
        if (k == 0) return (seq_t)1;
        if (k == 1) return (seq_t)2;
        return (seq_t)1;           /* k == 2 */
    }

    /* allocate row 0 and row 1 */
    seq_t *prev2 = (seq_t *)malloc(sizeof(seq_t) * 1);  /* row 0 */
    prev2[0] = (seq_t)1;
    seq_t *prev  = (seq_t *)malloc(sizeof(seq_t) * 3);  /* row 1 */
    prev[0] = (seq_t)1;
    prev[1] = (seq_t)2;
    prev[2] = (seq_t)1;

    /* build up to row r */
    for (idx_t i = 2; i <= r; ++i) {
        size_t len  = 2 * (size_t)i + 1;
        seq_t *curr = (seq_t *)malloc(sizeof(seq_t) * len);

        seq_t base   = (seq_t)4 * (seq_t)(i - 1);   /* 4*(i-1)   */
        seq_t coeff2 = (seq_t)2 * (seq_t)(i - 1);   /* 2*(i-1)   */

        idx_t prev_len  = 2 * (i - 1) + 1;          /* length(prev)  */
        idx_t prev2_len = 2 * (i - 2) + 1;          /* length(prev2) */

        for (idx_t m = 0; m < (idx_t)len; ++m) {
            seq_t t1 = 0, t2 = 0, t3 = 0;
            if (m < (size_t)prev_len) {
                t1 = base * prev[(size_t)m];
            }
            if (m >= 2 && (m - 2) < (size_t)prev_len) {
                t2 = base * prev[(size_t)(m - 2)];
            }
            if (m >= 2 && (m - 2) < (size_t)prev2_len) {
                t3 = coeff2 * prev2[(size_t)(m - 2)];
            }
            curr[(size_t)m] = t1 + t2 + t3;
        }

        free(prev2);      /* free row i-2 */
        prev2 = prev;     /* shift rows   */
        prev  = curr;
    }

    /* retrieve the desired coefficient */
    seq_t result = prev[(size_t)k];

    free(prev);   /* free row r   */
    free(prev2);  /* free row r-1 */

    return result;
}
