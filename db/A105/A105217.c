/*
 * A105217  Let b(n) denote the Lucas numbers, A000032: a(n) =
 *          Sum{k=0..n}C(n,k)^2*(n-k)!*b(k).
 * https://oeis.org/A105217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)^2 * (n-k)! * L(k) where L(k) is the
 *            k-th Lucas number (L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* allocate array for Lucas numbers up to n */
    seq_t *L = (seq_t *)malloc(((idx_t)n + 1) * sizeof(seq_t));
    if (L == NULL)
        return 0;                      /* out of memory – unlikely in tests */

    /* compute Lucas numbers */
    L[0] = (seq_t)2;
    if (n >= 1)
        L[1] = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        L[i] = L[i-1] + L[i-2];

    /* factorial of n */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    /* start iterative sum */
    seq_t binom = 1;                 /* C(n,0) */
    seq_t fact_current = fact;       /* (n-k)! when k is the loop variable */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = binom * binom;    /* C(n,k)^2 */
        term *= fact_current;          /* (n-k)!  */
        term *= L[k];                  /* L(k)    */
        result += term;

        if (k < n) {
            /* next binomial coefficient C(n,k+1) = C(n,k) * (n-k) / (k+1) */
            binom = (binom * (seq_t)(n - k)) / (seq_t)(k + 1);
            /* next factorial (n-(k+1))! = (n-k)! / (n-k) */
            fact_current /= (seq_t)(n - k);
        }
    }

    free(L);
    return result;
}
