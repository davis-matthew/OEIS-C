/*
 * A195177  a(n) = 3*n - floor(2*n*sqrt(2)).
 * https://oeis.org/A195177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n - floor(2*n*sqrt(2))
 * Range:     u8 n <= 1486, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Sequence values are computed with integer arithmetic only.    */
/* We use a fixed rational upper bound for sqrt(2) :             */
/*  sqrt(2) < SQ2_NUM / SQ2_DEN   with                                          */
static const uint64_t SQ2_NUM = 1414213563ULL;   /* ceil( sqrt(2) * 1e9 )   */
static const uint64_t SQ2_DEN = 1000000000ULL;   /* 1e9                     */

/* Return the n‑th term of OEIS A195177 for any non‑negative n.  */
seq_t a(idx_t n)
{
    /* Special case */
    if (n == 0)
        return (seq_t)0;

    /* 2*n is used frequently, keep it in 128‑bit to avoid overflow   */
    unsigned __int128 two_n = (unsigned __int128)2 * (unsigned __int128)n;

    /*  Approximate floor(2*n*sqrt(2)) using the rational over‑bound  */
    unsigned __int128 product = two_n * (unsigned __int128)SQ2_NUM;
    unsigned __int128 k = product / SQ2_DEN;          /* floor of the overestimate */

    /*  We now correct k, which may be at most one too large.       */
    /*  The true value satisfies:  k <= 2*n*sqrt(2) < k+1                */
    /*  Square both sides to stay in integer domain:                */
    /*      k*k   <= 8*n*n < (k+1)*(k+1)                              */
    unsigned __int128 left = (unsigned __int128)8 * (unsigned __int128)n * (unsigned __int128)n;

    /*  If k was over‑estimated, decrease it.                       */
    while (k * k > left)
        --k;

    /*  If k was underestimated, increase it.                       */
    while ((k + 1) * (k + 1) <= left)
        ++k;

    /*  Now k is exactly floor(2*n*sqrt(2)).                       */
    unsigned __int128 result = (unsigned __int128)3 * (unsigned __int128)n - k;

    return (seq_t)result;
}
