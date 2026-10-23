/*
 * A112093  Numerator of 3*Sum_{i=1..n} 1/(i^2*C(2*i,i)).
 * https://oeis.org/A112093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of 3*Sum_{i=1..n} 1/(i^2*C(2*i,i))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   gcd_any, mul128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_any.h"
#include "../utils/mul128.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t N = (seq_t)0;      /* running numerator of the partial sum 3*∑ 1/(i²*C(2i,i)) */
    seq_t D = (seq_t)1;      /* running denominator of that fraction */
    seq_t bi = (seq_t)1;     /* C(2*i,i), starts with C(0,0)=1 for i=0 */

    for (idx_t i = 1; i <= n; ++i) {
        /* -------- compute binomial C(2*i,i) incrementally -------- */
        seq_t i_s     = (seq_t)i;
        seq_t two_i   = i_s + i_s;                     /* 2*i          */
        seq_t two_i_m1= two_i - (seq_t)1;              /* 2*i-1        */
        seq_t i_sq    = i_s * i_s;                     /* i^2          */

        bi = mul128( mul128(bi, two_i), two_i_m1 );    /* bi *= 2*i*(2*i-1) */
        bi = bi / i_sq;                               /* bi /= i^2      */

        /* -------- term : 3/(i²*bi) -------- */
        seq_t term_den = mul128(i_sq, bi);            /* denominator of the term */
        seq_t g_term   = gcd_any( (seq_t)3, term_den );
        seq_t term_num = ((seq_t)3) / g_term;         /* numerator after reduction */
        seq_t term_den_red = term_den / g_term;       /* denominator after reduction */

        /* -------- add term to current fraction N/D -------- */
        /* Cross‑cancel common factors between term_den_red and D */
        seq_t g_cd = gcd_any(term_den_red, D);
        seq_t denom_part = term_den_red / g_cd;        /* term_den_red/g */
        seq_t D_div_g    = D / g_cd;                   /* D/g           */

        /* New numerator = N * denom_part + term_num * D_div_g */
        seq_t part1 = mul128(N, denom_part);
        seq_t part2 = mul128(term_num, D_div_g);
        seq_t newN  = part1 + part2;

        /* New denominator = D_div_g * term_den_red (= D/g * term_den_red) */
        seq_t newD  = mul128(D_div_g, term_den_red);

        /* Reduce the new fraction */
        seq_t g_reduce = gcd_any(newN, newD);
        N = newN / g_reduce;
        D = newD / g_reduce;
    }

    return N;   /* numerator of 3*partial sum in lowest terms */
}
