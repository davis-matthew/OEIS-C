/*
 * A050475  Numbers k such that x = 2^k-2 satisfies phi(x)+2 = phi(x+2).
 * https://oeis.org/A050475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n + 1 where p_n is the n‑th exponent of a Mersenne
 *            prime (2^p – 1 is prime). The exponents are known a long time in
 *            advance
 * Range:     u8 n <= 12, u16 n <= 27, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Known exponents of Mersenne primes (A000043).  The list contains all
   exponents discovered up to the present. */
static const idx_t ms_exponents[] = {
    2,   3,   5,   7,  13,  17,  19,  31,  61,  89, 107, 127,
  521, 607, 1279,2203,2281,3217,4253,4423,9689,9941,11213,19937,
 21701,23209,44497,86243,110503,132049,216091,756839,859433,1257787,
 1398269,2976221,3021377,6972593,13466917,20996011,24036583,25964951,
 30402457,32582657,37156667,42643801,43112609,57885161,74207281,77232917,
 82589933
};

/* Number of known exponents in the table. */
static const idx_t ms_count = sizeof(ms_exponents) / sizeof(ms_exponents[0]);

seq_t a(idx_t n)
{
    if (n < 1 || n > ms_count) {
        /* If the index is out of bounds (e.g. beyond currently known
           Mersenne primes) return 0 to indicate “not defined”. */
        return (seq_t)0;
    }
    /* a(n) = exponent + 1 */
    return (seq_t)ms_exponents[n - 1] + 1;
}
