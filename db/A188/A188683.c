/*
 * A188683  Alternate partial sums of binomial(3n,n)^2/(2n+1).
 * https://oeis.org/A188683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=0..n} binomial(3k,k)^2 * (-1)^{n-k} / (2k+1)
 *            Recurrence: 4*(n+2)^2*(4*n^2+16*n+15)*a(n+2) -
 *            (713*n^4+4246*n^3+9421*n^2+9224*n+3360)*a(n+1) -
 *            9*(9*n^2+27*n+20)^2*a(n) = 0 Base values: a(0)=1, a(1)=2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <type_traits>
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t a0 = (seq_t)1;   // a(n)
    seq_t a1 = (seq_t)2;   // a(n+1)

    constexpr bool is_integral = std::is_integral<seq_t>::value;

    for (idx_t i = 0; i <= n - 2; ++i)
    {
        if constexpr (is_integral)            // 64‑bit or smaller
        {
            using u128 = unsigned __int128;
            u128 it   = (u128)i;
            u128 it2  = it * it;
            u128 it3  = it2 * it;
            u128 it4  = it2 * it2;

            u128 coeff1 = 713 * it4 + 4246 * it3 + 9421 * it2 + 9224 * it + 3360;
            u128 t      = 9 * it2 + 27 * it + 20;
            u128 t2     = t * t;

            u128 term1 = coeff1 * (u128)a1;
            u128 term2 = 9 * t2 * (u128)a0;
            u128 numerator = term1 + term2;

            u128 denom = 4 * (u128)(i + 2) * (u128)(i + 2) *
                         (4 * it2 + 16 * it + 15);

            a0 = a1;
            a1 = (seq_t)(numerator / denom);   // result fits in seq_t
        }
        else                                   // arbitrary‑precision type
        {
            seq_t it   = (seq_t)i;
            seq_t it2  = it * it;
            seq_t it3  = it2 * it;
            seq_t it4  = it2 * it2;

            seq_t coeff1 = (seq_t)713 * it4 + (seq_t)4246 * it3 +
                           (seq_t)9421 * it2 + (seq_t)9224 * it + (seq_t)3360;
            seq_t t      = (seq_t)9 * it2 + (seq_t)27 * it + (seq_t)20;
            seq_t t2     = t * t;

            seq_t term1 = coeff1 * a1;
            seq_t term2 = (seq_t)9 * t2 * a0;
            seq_t numerator = term1 + term2;

            seq_t denom = (seq_t)4 * (seq_t)(i + 2) * (seq_t)(i + 2) *
                          ((seq_t)4 * it2 + (seq_t)16 * it + (seq_t)15);

            a0 = a1;
            a1 = numerator / denom;   // exact division
        }
    }

    return a1;   // a(n)
}
#else   /* plain C: seq_t is always an unsigned integral type */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)2;

    for (idx_t i = 0; i <= n - 2; ++i)
    {
        unsigned __int128 it    = (unsigned __int128)i;
        unsigned __int128 it2   = it * it;
        unsigned __int128 it3   = it2 * it;
        unsigned __int128 it4   = it2 * it2;

        unsigned __int128 coeff1 = 713*it4 + 4246*it3 + 9421*it2 + 9224*it + 3360;
        unsigned __int128 t      = 9*it2 + 27*it + 20;
        unsigned __int128 t2     = t * t;

        unsigned __int128 term1 = coeff1 * (unsigned __int128)a1;
        unsigned __int128 term2 = 9 * t2 * (unsigned __int128)a0;
        unsigned __int128 numerator = term1 + term2;

        unsigned __int128 denom = 4 * (unsigned __int128)(i + 2) *
                                  (unsigned __int128)(i + 2) *
                                  (4 * it2 + 16 * it + 15);

        a0 = a1;
        a1 = (seq_t)(numerator / denom);
    }

    return a1;
}
#endif
