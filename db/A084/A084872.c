/*
 * A084872  Number of 5-multiantichains of an n-set.
 * https://oeis.org/A084872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5!)*(32^n - 20*24^n + 60*20^n + 20*18^n + 10*17^n * -
 *            90*16^n - 120*15^n + 150*14^n + 120*13^n - 480*12^n * + 20*11^n
 *            + 720*10^n + 120*9^n - 445*8^n + 180*7^n * - 1650*6^n + 1650*5^n
 *            + 870*4^n - 1740*3^n + 744*2^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   pow128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow128.h"

#ifndef __cplusplus

#endif

#ifdef __cplusplus
/* Exponentiation by squaring for big integers (C++ builds). */
static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1u)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}
#endif

seq_t a(idx_t n)
{
    /* Base values and their integer coefficients from the OEIS formula. */
    static const int bases[20]  = {32, 24, 20, 18, 17, 16, 15, 14, 13,
                                   12, 11, 10,  9,  8,  7,  6,  5,  4,
                                   3,  2};
    static const int coeffs[20] = { 1, -20, 60, 20, 10, -90, -120, 150,
                                   120, -480, 20, 720, 120, -445, 180,
                                   -1650, 1650, 870, -1740, 744};

#ifndef __cplusplus
    /* C implementation using 128‑bit intermediates (works for uint64_t
     * builds up to the point where the value fits in uint64_t). */
    __int128 sum = 0;
    for (int i = 0; i < 20; ++i)
    {
        __int128 p = pow128(bases[i], n);
        sum += (__int128)coeffs[i] * p;
    }
    return (seq_t)(sum / 120);
#else
    /* C++ implementation using arbitrary‑precision seq_t (no overflow). */
    seq_t sum = 0;
    for (int i = 0; i < 20; ++i)
    {
        seq_t p = pow_seq_t((seq_t)bases[i], n);
        sum += (seq_t)coeffs[i] * p;
    }
    return sum / (seq_t)120;
#endif
}
