/*
 * A233208  A measure of quality (the higher the better) for the approximation
 *          to e by rationals A022852(n)/n.
 * https://oeis.org/A233208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( 1 / abs( n*e - nearest_integer(n*e) ) )
 * Range:     u8 none, u16 none, u32 none, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the nearest integer to n*e using a rational
 * approximation of e with relative error less than 3·10⁻¹¹ for n ≤ 10⁴.
 * The approximation is based on the 16! factorial which fits into a 64‑bit
 * unsigned integer.  All calculations are performed with integer arithmetic
 * only, avoiding any floating‑point operations. */

#include <stdint.h>
#include <stdbool.h>

static const uint64_t FACT[17] = {
    1ULL,                     /* 0! */
    1ULL,                     /* 1! */
    2ULL,                     /* 2! */
    6ULL,                     /* 3! */
    24ULL,                    /* 4! */
    120ULL,                   /* 5! */
    720ULL,                   /* 6! */
    5040ULL,                  /* 7! */
    40320ULL,                 /* 8! */
    362880ULL,                /* 9! */
    3628800ULL,               /*10! */
    39916800ULL,              /*11! */
    479001600ULL,             /*12! */
    6227020800ULL,            /*13! */
    87178291200ULL,           /*14! */
    1307674368000ULL,         /*15! */
    20922789888000ULL         /*16! */
};

seq_t a(idx_t n)
{
    /* Lazy initialization of the rational approximation of e. */
    static bool initialized = false;
    static uint64_t e_num;   /* numerator of e ≈ e_num / e_den */
    static uint64_t e_den;   /* denominator of e ≈ e_num / e_den */

    if (!initialized) {
        e_den = FACT[16];   /* 16! */
        uint64_t sum = 0;
        for (idx_t k = 0; k <= 16; ++k) {
            sum += e_den / FACT[k];
        }
        e_num = sum;
        initialized = true;
    }

    /* Compute n * e as a rational number num / e_den. */
    __int128 num = (__int128)n * (__int128)e_num;     /* numerator of n*e */

    /* Determine the nearest integer m to n*e. */
    __int128 half = e_den / 2;
    __int128 m = (num + half) / e_den;                /* round to nearest */

    /* Compute the absolute difference numerator: |n*e - m| * e_den. */
    __int128 diff = num - m * (__int128)e_den;
    if (diff < 0) diff = -diff;

    /* The result is floor(e_den / diff) = floor(1 / (diff / e_den)). */
    seq_t result = (seq_t)(e_den / (seq_t)diff);
    return result;
}
