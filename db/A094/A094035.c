/*
 * A094035  Number of connected 4-element antichains on a labeled n-set.
 * https://oeis.org/A094035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (exp(15*n) - 12*exp(11*n) + 24*exp(9*n) - 14*exp(7*n) +
 *            27*exp(6*n) - 60*exp(5*n) - 24*exp(4*n) + 155*exp(3*n) -
 *            141*exp(2*n) + 50*exp(n) - 6) / 24
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 17, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    static const idx_t ks[11]  = {15, 11, 9, 7, 6, 5, 4, 3, 2, 1, 0};
    static const int  coeffs[11] = { 1, -12, 24, -14, 27, -60, -24, 155, -141, 50, -6};

    /* If seq_t is at most 64 bits, use a 128‑bit intermediate to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 pos = 0, neg = 0;
        for (int i = 0; i < 11; ++i) {
            unsigned __int128 power = 1;
            for (idx_t j = 0; j < n; ++j) power *= (unsigned __int128)ks[i];
            int c = coeffs[i];
            unsigned __int128 term = power * (unsigned __int128)(c >= 0 ? c : -c);
            if (c >= 0) pos += term; else neg += term;
        }
        unsigned __int128 result = pos - neg;
        result /= 24;
        return (seq_t)result;
    }

    /* Full arbitrary‑precision calculation for larger seq_t types. */
    seq_t pos = 0, neg = 0;
    seq_t exp = (seq_t)n;
    for (int i = 0; i < 11; ++i) {
        seq_t power = big_pow((seq_t)ks[i], exp);
        int c = coeffs[i];
        seq_t term = power * (seq_t)(c >= 0 ? c : -c);
        if (c >= 0) pos += term; else neg += term;
    }
    seq_t result = pos;
    result -= neg;
    result /= (seq_t)24;
    return result;
}
