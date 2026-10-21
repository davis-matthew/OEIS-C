/*
 * A051182  Number of 5-element intersecting families of an n-element set.
 * https://oeis.org/A051182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            (32^n-10*24^n+30*20^n-5*18^n+5*17^n-80*16^n-30*15^n+135*14^n+30*13^n-80*12^n-2*11^n+10*10^n-100*9^n+240*8^n-160*7^n-44*6^n+95*5^n-85*4^n+50*3^n+24*2^n-24)
 *            / 120
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/big_pow.h"

/* fast exponentiation for 128‑bit intermediate calculations */
static inline __int128 pow128(unsigned long long base, idx_t exp)
{
    __int128 result = 1;
    __int128 b = base;
    while (exp) {
        if (exp & 1ULL) result *= b;
        exp >>= 1ULL;
        if (exp) b *= b;
    }
    return result;
}

/* coefficients and bases for the terms in the numerator */
static const struct {
    unsigned long long base;
    int coeff;
} terms[] = {
    {32ULL,  1},
    {24ULL, -10},
    {20ULL,  30},
    {18ULL,  -5},
    {17ULL,   5},
    {16ULL, -80},
    {15ULL, -30},
    {14ULL, 135},
    {13ULL,  30},
    {12ULL, -80},
    {11ULL,  -2},
    {10ULL,  10},
    {9ULL,  -100},
    {8ULL,   240},
    {7ULL,  -160},
    {6ULL,   -44},
    {5ULL,    95},
    {4ULL,   -85},
    {3ULL,    50},
    {2ULL,    24}
};

seq_t a(idx_t n)
{
    /* builtin integer types (up to 64‑bit) use 128‑bit arithmetic */
    if (sizeof(seq_t) <= 8) {
        __int128 sum = 0;
        for (size_t i = 0; i < sizeof(terms)/sizeof(terms[0]); ++i) {
            __int128 p = pow128(terms[i].base, n);
            sum += (__int128)terms[i].coeff * p;
        }
        sum -= 24;                /* constant term */
        return (seq_t)(sum / 120); /* division is exact for all n >= 0 */
    }

    /* arbitrary‑precision build */
    seq_t pos = 0;
    seq_t neg = 0;
    for (size_t i = 0; i < sizeof(terms)/sizeof(terms[0]); ++i) {
        seq_t p = big_pow((seq_t)terms[i].base, (seq_t)n);
        if (terms[i].coeff >= 0) {
            pos += p * (seq_t)terms[i].coeff;
        } else {
            neg += p * (seq_t)(-terms[i].coeff);
        }
    }
    neg += 24;                 /* constant term */
    seq_t result = pos - neg;  /* pos >= neg for n >= 0 */
    result /= 120;
    return result;
}
