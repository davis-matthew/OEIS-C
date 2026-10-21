/*
 * A091699  Row sums of triangle A091698.
 * https://oeis.org/A091699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 17, i64 n <= 31, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 2*n*a(n) + (13*n-16)*a(n-1) + 4*(4*n-7)*a(n-2) + 5*(n-2)*a(n-3) = 0
   Initial values: a(0)=1, a(1)=0, a(2)=-1 */

#ifdef __cplusplus
#include <type_traits>

seq_t a(idx_t n)
{
    /* handle the first three terms */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)0;
    if (n == 2u) return (seq_t)-1;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)0;   /* a(n-2) */
    seq_t a2 = (seq_t)-1;  /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i)
    {
        if constexpr (std::is_integral<seq_t>::value) {
            /* Integral seq_t: use 128‑bit intermediate */
            __int128 coeff1 = (__int128)13 * i - 16;
            __int128 coeff2 = (__int128)4 * ( (__int128)4 * i - 7 );
            __int128 coeff3 = (__int128)5 * ( i - 2 );

            __int128 num = - ( coeff1 * (__int128)a2
                               + coeff2 * (__int128)a1
                               + coeff3 * (__int128)a0 );
            __int128 denom = (__int128)2 * i;
            seq_t an = (seq_t)(num / denom);

            /* shift window */
            a0 = a1; a1 = a2; a2 = an;
        } else {
            /* Arbitrary‑precision seq_t: use seq_t operations */
            seq_t coeff1 = (seq_t)13 * (seq_t)i - (seq_t)16;
            seq_t coeff2 = (seq_t)4 * ( (seq_t)4 * (seq_t)i - (seq_t)7 );
            seq_t coeff3 = (seq_t)5 * ( (seq_t)i - (seq_t)2 );

            seq_t num = - ( coeff1 * a2
                            + coeff2 * a1
                            + coeff3 * a0 );
            seq_t denom = (seq_t)2 * (seq_t)i;
            seq_t an = num / denom;

            /* shift window */
            a0 = a1; a1 = a2; a2 = an;
        }
    }
    return a2;
}
#else
/* C implementation – seq_t is a signed integer type */
seq_t a(idx_t n)
{
    /* handle the first three terms */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)0;
    if (n == 2u) return (seq_t)-1;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)0;   /* a(n-2) */
    seq_t a2 = (seq_t)-1;  /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i)
    {
        __int128 coeff1 = (__int128)13 * i - 16;
        __int128 coeff2 = (__int128)4 * ( (__int128)4 * i - 7 );
        __int128 coeff3 = (__int128)5 * ( i - 2 );

        __int128 num = - ( coeff1 * (__int128)a2
                           + coeff2 * (__int128)a1
                           + coeff3 * (__int128)a0 );
        __int128 denom = (__int128)2 * i;
        seq_t an = (seq_t)(num / denom);

        /* shift window */
        a0 = a1; a1 = a2; a2 = an;
    }
    return a2;
}
#endif
