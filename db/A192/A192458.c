/*
 * A192458  a(n) = A192457(n)/2.
 * https://oeis.org/A192458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n * (n+1)! - (2n-1)!!) / 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* compute n!  (sequentially, using seq_t arithmetic)  */
static seq_t factorial_seq(idx_t k)
{
    seq_t f = 1;
    for (idx_t i = 2; i <= k; ++i)
        f *= (seq_t)i;
    return f;
}

/* compute 2^n  by left‑shifting in a loop */
static seq_t pow2_seq(idx_t n)
{
    seq_t p = 1;
    for (idx_t i = 0; i < n; ++i)
        p <<= 1;
    return p;
}

/* compute odd double factorial (2n-1)!!  */
static seq_t odd_double_fact(idx_t n)
{
    seq_t d = 1;
    for (idx_t k = 1; k <= n; ++k)
        d *= (seq_t)(2 * k - 1);
    return d;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* a(0) = 0 */
    if (n == 1) return 1;           /* a(1) = 1 */

    /* compute (n+1)! and divide by 3 first */
    seq_t fact = factorial_seq(n + 1);
    fact /= 3;                      /* (n+1)! / 3  (exact for n>=2) */

    /* compute 2^n */
    seq_t p = pow2_seq(n);

    /* term1 = 2^n * ((n+1)! / 3) */
    seq_t term1 = p * fact;

    /* compute odd double factorial and divide by 3 */
    seq_t odd = odd_double_fact(n);
    odd /= 3;                       /* (2n-1)!! / 3  (exact for n>=2) */

    /* a(n) = term1 - odd */
    return term1 - odd;
}
