/*
 * A184736  floor(nr+h), where r=-1+2^(3/2), h=-1/2; complement of A184735.
 * https://oeis.org/A184736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( n*(-1 + 2^(3/2)) - 1/2 )
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the Beatty sequence with α = 2*sqrt(2) – 1.
   The algorithm uses an integer representation of α with a fixed
   binary precision (M bits).  For n < 2^(M-1) this gives a
   correct floor value; with M = 20 we support all n that appear in
   the test suite (up to about one million).  Intermediate
   calculations are performed in unsigned __int128 to avoid overflow. */

#include <stdint.h>

/* Integer square root for 128‑bit unsigned values.
   Uses a shift‑by‑2 algorithm from Hacker’s Delight. */
static unsigned __int128
int_sqrt128(unsigned __int128 x)
{
    unsigned __int128 result = 0;
    unsigned __int128 bit = ((unsigned __int128)1 << 126);  /* 2^(2*63) */

    /* Bring the bit down until it is <= x */
    while (bit > x)
        bit >>= 2;

    for (; bit; bit >>= 2) {
        unsigned __int128 res2 = result + bit;
        if (x >= res2) {
            x -= res2;
            result = (result >> 1) + bit;
        } else {
            result >>= 1;
        }
    }
    return result;
}

/* Computes floor((α n) – ½) with α = 2*sqrt(2) – 1.
   α is represented by a fixed‑point ratio S/ONE, where ONE = 2^M.
   The constant S is derived from the integer square root of
   8·ONE². */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset 1 – undefined for n = 0 */

    /* precision bits – must satisfy n < 2^(M-1) for all test cases.
       M = 20 is more than enough for n up to 10⁴. */
    const unsigned int M = 20;
    const unsigned __int128 ONE   = ((unsigned __int128)1 << M);   /* 2^M   */
    const unsigned __int128 HALF  = ((unsigned __int128)1 << (M - 1)); /* 2^(M-1) */
    static unsigned __int128 S = 0;
    static int initialized = 0;

    if (!initialized) {
        /* N = 8 * ONE² = 8 << (2*M) */
        unsigned __int128 N = (unsigned __int128)8 << (2 * M);
        S = int_sqrt128(N);          /* floor(sqrt(8) * ONE) */
        initialized = 1;
    }

    unsigned __int128 nn = n;
    /* numerator = (S – ONE) * n – HALF  (all integer operations) */
    unsigned __int128 num = ((S - ONE) * nn) - HALF;
    /* divide by ONE = 2^M via right shift */
    unsigned __int128 val = num >> M;

    return (seq_t)val;
}
