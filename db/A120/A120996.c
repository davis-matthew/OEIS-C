/*
 * A120996  Numerators of partial sums of Catalan numbers scaled by powers of
 *          1/9.
 * https://oeis.org/A120996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of Σ_{k=0..n} Catalan(k)/9^k in lowest terms.
 *            Let t_0 = 1. For n ≥ 1 let t_n = 9 * t_{n-1} + Catalan(n). Then
 *            a(n) = t_n / 3^{v_3(t_n)}, where v_3(x) is the exponent of 3 in
 *            x
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are supplied by the build. */
seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 1 */
        return (seq_t)1;

    seq_t t  = (seq_t)1;          /* current t_{k-1}  (initially t_0) */
    seq_t cat = (seq_t)1;         /* Catalan(k-1), initially C(0) */
    seq_t result;                 /* will hold a(n) */

    for (idx_t i = 1; i <= n; ++i) {
        /* compute Catalan(i) from Catalan(i-1) :
           C(i) = C(i-1) * (4*(i-1)+2) / (i+1)   (integer division) */
        idx_t mul = 4 * (i - 1) + 2;   /* 4*(i-1)+2 = 4i-2 */
        seq_t cat_next = cat * (seq_t)mul / (idx_t)(i + 1);

        /* update t_i = 9 * t_{i-1} + C(i) */
        t = t * (seq_t)9 + cat_next;

        /* if we have reached the requested index, compute the numerator */
        if (i == n) {
            result = t;
            /* divide by the largest power of 3 that divides t */
            while ((result % (seq_t)3) == (seq_t)0)
                result /= (seq_t)3;
            return result;
        }

        /* prepare for next iteration */
        cat = cat_next;
    }

    /* should never reach here */
    return (seq_t)0;
}
