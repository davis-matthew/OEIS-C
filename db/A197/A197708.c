/*
 * A197708  a(n) = floor((n + 1/n)^7).
 * https://oeis.org/A197708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + 1/n)^7)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 23, u64 n <= 565,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n > 35 this equals n^7 + 7*n^5 + 21*n^3 + 35*n. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate type for all arithmetic.
       This type is available on GCC/Clang compilers that are
       used for the judging system. */
    typedef unsigned __int128 u128;

    /* Handle the small range with the exact rational formula.
       For n ≤ 35 the numerator (n^2+1)^7 may overflow 64 bits,
       so we compute it in u128 and then divide by n^7. */
    if (n <= 35)
    {
        u128 denom = 1;
        for (int i = 0; i < 7; ++i)
            denom *= (u128)n;

        u128 x = (u128)n * (u128)n + 1;   /* n^2 + 1 */
        u128 numer = 1;
        for (int i = 0; i < 7; ++i)
            numer *= x;

        return (seq_t)(numer / denom);   /* integer division = floor */
    }

    /* For larger n use the polynomial expression. */
    u128 nn = (u128)n;          /* cast index to 128‑bit */
    u128 n2 = nn * nn;
    u128 n3 = n2 * nn;
    u128 n5 = n3 * n2;
    u128 n7 = n5 * n2;

    u128 result = n7 + (u128)7 * n5 + (u128)21 * n3 + (u128)35 * nn;
    return (seq_t)result;
}
