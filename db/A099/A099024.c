/*
 * A099024  a(n) = A033466(5n+2). Values of A033466(n) that differ from
 *          A058031(n+1)+1.
 * https://oeis.org/A099024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator of (2*n+1)/((n^2+(2*n+1)^2)*(1+(5*n+3)^2))
 *            The denominator after cancelling common factors with the
 *            numerator
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 77, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Greatest‑common divisor for unsigned 128‑bit integers. */
static inline unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b != 0) {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Promote to 128‑bit to keep all intermediate results exact. */
    unsigned __int128 nn = (unsigned __int128)n;

    /* 2*n + 1 (the numerator of the fraction). */
    unsigned __int128 N = 2 * nn + 1;

    /* n^2 + (2*n + 1)^2  == n^2 + N^2. */
    unsigned __int128 term1 = nn * nn + N * N;

    /* 1 + (5*n + 3)^2. */
    unsigned __int128 t5 = 5 * nn + 3;
    unsigned __int128 term2 = 1 + t5 * t5;

    /* Full denominator before reduction. */
    unsigned __int128 denom = term1 * term2;

    /* Reduce with the numerator. */
    unsigned __int128 g = gcd128(N, denom);
    unsigned __int128 result = denom / g;

    return (seq_t)result;
}
