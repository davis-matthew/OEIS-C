/*
 * A116759  Number of permutations of length n which avoid the patterns 2134,
 *          3421, 4123.
 * https://oeis.org/A116759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 + 5*n^5 - 55*n^4 + 195*n^3 - 186*n^2 + 40*n +
 *            240)/240
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic when available to avoid overflow on large n. */
#if defined(__SIZEOF_INT128__)
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n2 = nn * nn;
    unsigned __int128 n3 = n2 * nn;
    unsigned __int128 n4 = n3 * nn;
    unsigned __int128 n5 = n4 * nn;
    unsigned __int128 n6 = n5 * nn;

    /* Positive and negative parts of the numerator, built to avoid
       subtraction underflow.                           */
    unsigned __int128 pos = n6 + 5 * n5 + 195 * n3 + 40 * nn + 240;
    unsigned __int128 neg = 55 * n4 + 186 * n2;
    unsigned __int128 num = pos - neg;     /* guaranteed non‑negative */

    unsigned __int128 res = num / 240;
    return (seq_t)res;
#else
    /* Fallback for systems without 128‑bit support.
       Computes the same expression using only 64‑bit arithmetic.
       This path is safe when the intermediate values fit in 64 bits
       (i.e., for n ≤ 1600).                            */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long n2 = nn * nn;
    unsigned long long n3 = n2 * nn;
    unsigned long long n4 = n3 * nn;
    unsigned long long n5 = n4 * nn;
    unsigned long long n6 = n5 * nn;

    unsigned long long pos = n6 + 5 * n5 + 195 * n3 + 40 * nn + 240;
    unsigned long long neg = 55 * n4 + 186 * n2;
    unsigned long long num = pos - neg;
    return (seq_t)(num / 240);
#endif
}
