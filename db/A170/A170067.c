/*
 * A170067  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: • a(0) = 1
   • for 1 ≤ n ≤ 36:  a(n) = 10·9^(n‑1)
   • for n ≥ 37:      a(n) = 8·∑_{k=1}^{36} a(n‑k) – 36·a(n‑37) + (n==37?1:0)  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* special case n==0 */
    if (n == 0) return (seq_t)1;

    /* allocate array to hold terms up to n */
    size_t len = (size_t)n + 1;
    seq_t *a = (seq_t *)malloc(len * sizeof(seq_t));
    if (!a) return (seq_t)0;  /* allocation failed */

    a[0] = (seq_t)1;

    /* compute terms 1 … min(n,36) by the simple formula */
    seq_t pow9 = (seq_t)1;       /* 9^(i-1) */
    idx_t i;
    for (i = 1; i <= n && i <= 36; ++i)
    {
        a[i] = (seq_t)10 * pow9;
        pow9 *= (seq_t)9;
    }

    /* if n ≤ 36 we are done */
    if (n <= 36)
    {
        seq_t res = a[n];
        free(a);
        return res;
    }

    /* array of b(n) values for the generating function */
    /* b(0)=1, b(1..36)=2, b(37)=1, rest 0 */
    seq_t b[38];
    b[0] = b[37] = (seq_t)1;
    for (idx_t k = 1; k <= 36; ++k) b[k] = (seq_t)2;
    for (idx_t k = 38; k < 38; ++k) b[k] = (seq_t)0;  /* just for completeness */

    /* compute terms 37 … n using the recurrence */
    for (i = 37; i <= n; ++i)
    {
        seq_t sum = (seq_t)0;
        for (int k = 1; k <= 36; ++k)
            sum += a[i - k];

        seq_t term = sum * (seq_t)8;
        term -= a[i - 37] * (seq_t)36;   /* subtract the a[n-37] part */
        if (i == 37) term += b[37];      /* add the constant 1 for n==37 */

        a[i] = term;
    }

    seq_t result = a[n];
    free(a);
    return result;
}
