/*
 * A008761  Expansion of (1+x^18)/((1-x)*(1-x^2)*(1-x^3)).
 * https://oeis.org/A008761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(n) + f(n-18), where f(n) is the number of non‑negative
 *            integer solutions of a + 2b + 3c = n. This is the coefficient of
 *            x^n in the expansion of (1+x^18)/((1-x)(1-x^2)(1-x^3))
 * Range:     u8 n <= 44, u16 n <= 633, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* h(n) = number of solutions of 2b + 3c = n,  b,c >= 0 */
static inline seq_t h(idx_t n)
{
    idx_t m      = n / 3;          /* floor(n/3) */
    idx_t parity = n % 2;          /* 0 or 1 */

    if (parity > m)
        return (seq_t)0;

    /* (m - parity)/2 + 1 */
    return (seq_t)((m - parity) / 2 + 1);
}

/* f(n) = number of non‑negative solutions of a + 2b + 3c = n,
   which satisfies f(0)=1 and f(n)=f(n-1)+h(n) for n>0. */
static seq_t compute_f(idx_t n)
{
    seq_t f = (seq_t)1;            /* f(0) */
    for (idx_t i = 1; i <= n; ++i)
        f += h(i);
    return f;
}

/* The requested sequence */
seq_t a(idx_t n)
{
    seq_t res = compute_f(n);
    if (n >= 18)
        res += compute_f(n - 18);
    return res;
}
