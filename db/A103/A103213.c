/*
 * A103213  a(n) = n! * Sum_{k=1..n} binomial(n,k)/k.
 * https://oeis.org/A103213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=1..n} binomial(n,k)/k Recurrence (for n ≥ 5):
 *            a(n) = 2*(n-1)*(n-2)^2*a(n-3) - (n-1)*(13+5*(n-4))*a(n-2) +
 *            (4*(n-4)+13)*a(n-1) Initial values (OEIS): a(1)=1, a(2)=5,
 *            a(3)=29, a(4)=206
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t mul_small(seq_t a, idx_t b)
{
    /* multiply by a small integer; overflow on small seq_t types is acceptable for those builds */
    return a * (seq_t)b;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, but return 0 for n==0 just in case */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)29;
    if (n == 4) return (seq_t)206;

    /* sliding window of the last three terms:
       a_{i-3}, a_{i-2}, a_{i-1}   (initially a(2), a(3), a(4)) */
    seq_t a_im3 = (seq_t)5;   /* a(2) */
    seq_t a_im2 = (seq_t)29;  /* a(3) */
    seq_t a_im1 = (seq_t)206; /* a(4) */
    seq_t a_i    = 0;         /* will hold a(i) */

    for (idx_t i = 5; i <= n; ++i) {
        /* term1 = 2 * (i-1) * (i-2)^2 * a_{i-3} */
        seq_t term1 = mul_small(a_im3, 2);
        term1 = mul_small(term1, i-1);
        term1 = mul_small(term1, i-2);
        term1 = mul_small(term1, i-2);

        /* term2 = (i-1) * (13 + 5*(i-4)) * a_{i-2} */
        seq_t term2 = mul_small(a_im2, i-1);
        term2 = mul_small(term2, 13 + 5*(i-4));

        /* term3 = (4*(i-4) + 13) * a_{i-1} */
        seq_t term3 = mul_small(a_im1, 4*(i-4) + 13);

        a_i = term1 + term3 - term2;   /* a(i) */

        /* shift the window for next iteration */
        a_im3 = a_im2;
        a_im2 = a_im1;
        a_im1 = a_i;
    }

    return a_i;
}
