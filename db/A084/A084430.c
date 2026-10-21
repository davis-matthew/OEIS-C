/*
 * A084430  Smallest prime with "n^3" as central digit(s).
 * https://oeis.org/A084430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = A084413(n^3) (smallest prime whose decimal representation
 *            has the d‑digit block equal to n³ centrally placed, where d is
 *            the number of digits of n³.)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 220, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len_uint64, isPrime_u64, mod_mul_u64, mod_pow_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_len_uint64.h"
#include "../utils/isPrime_u64.h"
#include "../utils/mod_mul_u64.h"
#include "../utils/mod_pow_u64.h"

/* --- auxiliary: Miller–Rabin primality for 64‑bit integers --- */

/* 10^k for k = 0..40 as unsigned __int128 (enough for our use) */
static const unsigned __int128 pow10_uint128[41] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
    10000000000000000000ULL,
    100000000000000000000ULL,
    1000000000000000000000ULL,
    10000000000000000000000ULL,
    100000000000000000000000ULL,
    1000000000000000000000000ULL,
    10000000000000000000000000ULL,
    100000000000000000000000000ULL,
    1000000000000000000000000000ULL,
    10000000000000000000000000000ULL,
    100000000000000000000000000000ULL,
    1000000000000000000000000000000ULL,
    10000000000000000000000000000000ULL,
    100000000000000000000000000000000ULL,
    1000000000000000000000000000000000ULL,
    10000000000000000000000000000000000ULL,
    100000000000000000000000000000000000ULL,
    1000000000000000000000000000000000000ULL,
    10000000000000000000000000000000000000ULL,
    100000000000000000000000000000000000000ULL,
    1000000000000000000000000000000000000000ULL,
    10000000000000000000000000000000000000000ULL
};

/* --- main sequence function --- */
seq_t a(idx_t n) {
    /* n³ as seq_t and as unsigned 64-bit (fits for all n in test data) */
    unsigned __int128 n128 = (unsigned __int128)n;
    unsigned __int128 cube128 = n128 * n128 * n128;
    uint64_t cube64 = (uint64_t)cube128; /* safe for n up to 10⁴ */

    int d = digit_len_uint64(cube64);   /* digits of n³ */

    /* try increasing padding length t = 0,1,2,… */
    for (int t = 0; ; ++t) {
        unsigned __int128 pow_t   = pow10_uint128[t];          /* 10^t   */
        unsigned __int128 pow_dt  = pow10_uint128[d + t];      /* 10^(d+t) */
        unsigned __int128 pow_d_t = pow10_uint128[d + t];      /* same as pow_dt */

        unsigned __int128 left_low  = (t == 0) ? 0 : pow10_uint128[t-1];
        unsigned __int128 left_high = pow10_uint128[t] - 1;
        unsigned __int128 right_high = pow10_uint128[t] - 1;

        /* iterate over all left and right paddings in increasing order */
        for (unsigned __int128 left = left_low; left <= left_high; ++left) {
            for (unsigned __int128 right = 0; right <= right_high; ++right) {
                /* construct candidate: left | cube | right */
                unsigned __int128 cand128 = left * pow_dt + (unsigned __int128)cube64 * pow_t + right;

                /* candidate must fit in 64-bit for our primality test */
                if (cand128 > (unsigned __int128)UINT64_MAX) goto next_t; /* skip excessive larger numbers */

                uint64_t cand = (uint64_t)cand128;
                if (isPrime_u64(cand))
                    return (seq_t)cand;
            }
        }
    next_t: ; /* continue with next t */
    }
}
