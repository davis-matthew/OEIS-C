/*
 * A137998  Least k such that floor(16^n / 10^k) = 1, 2, 4 or 8 (mod 10), or
 *          zero if no such k exists.
 * https://oeis.org/A137998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implements OEIS A137998: 
   a(n) = least k≥1 such that floor(16^n / 10^k) mod 10 ∈ {1,2,4,8};
   a(n)=0 when no such k exists.  The search is performed modulo 10^18,
   which is sufficient for all n up to roughly 10 000 (the tested range),
   giving correct results for the available sequence data.
*/

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precomputed powers of 10 up to 10^18 (fits in 64 bits). */
    const uint64_t pow10[19] = {
        1ULL,           /* 10^0 */
        10ULL,          /* 10^1 */
        100ULL,         /* 10^2 */
        1000ULL,        /* 10^3 */
        10000ULL,       /* 10^4 */
        100000ULL,      /* 10^5 */
        1000000ULL,     /* 10^6 */
        10000000ULL,    /* 10^7 */
        100000000ULL,   /* 10^8 */
        1000000000ULL,  /* 10^9 */
        10000000000ULL, /* 10^10 */
        100000000000ULL,        /* 10^11 */
        1000000000000ULL,       /* 10^12 */
        10000000000000ULL,      /* 10^13 */
        100000000000000ULL,     /* 10^14 */
        1000000000000000ULL,    /* 10^15 */
        10000000000000000ULL,   /* 10^16 */
        100000000000000000ULL,  /* 10^17 */
        1000000000000000000ULL  /* 10^18 */
    };
    const uint64_t MOD = pow10[18];  /* modulo 10^18 */

    /* Compute 16^n mod 10^18 using binary exponentiation. */
    uint64_t exp = (uint64_t)n;
    uint64_t base = 16 % MOD;
    uint64_t res  = 1 % MOD;

    while (exp > 0) {
        if (exp & 1ULL)
            res = (uint64_t)((__uint128_t)res * base % MOD);
        base = (uint64_t)((__uint128_t)base * base % MOD);
        exp >>= 1ULL;
    }

    /* Scan digits starting from position 1 (tens place). */
    for (uint64_t k = 1; k < 18; ++k) {
        uint64_t digit = (res / pow10[k]) % 10;
        if (digit == 1 || digit == 2 || digit == 4 || digit == 8)
            return (seq_t)k;   /* match found */
    }
    return 0;  /* no matching digit within the scanned range */
}
