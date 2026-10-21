/*
 * A066642  a(n) = floor(n^(n/2)).
 * https://oeis.org/A066642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( n^(n/2) ) (OEIS A066642)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 46,
 *            big any n
 * Helpers:   big_pow, floor_sqrt, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
/* utilities that work on arbitrary‑precision seq_t types */
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/floor_sqrt.h"

/* Integer square root of a 128‑bit value (returns 64‑bit result).  */
static unsigned long long sqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 low  = 0;
    unsigned __int128 high = (((unsigned __int128)1) << 64) - 1;
    unsigned __int128 best = 0;

    while (low <= high) {
        unsigned __int128 mid   = (low + high) >> 1;
        unsigned __int128 square = mid * mid;

        if (square == x) {
            best = (unsigned long long)mid;
            break;
        }
        if (square < x) {
            best = (unsigned long long)mid;
            low  = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return (unsigned long long)best;
}

seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 by convention (0^0 = 1) */
    if (n == 0) return (seq_t)1;

    /* Small integer arithmetic when seq_t is 64‑bit or less */
    if (sizeof(seq_t) <= 8) {
        if ((n & 1ULL) == 0) {  /* even n: n^(n/2) is an integer */
            unsigned __int128 result = 1;
            unsigned __int128 base   = (unsigned __int128)n;
            idx_t exp = n / 2;
            while (exp) {
                if (exp & 1ULL) result *= base;
                base *= base;
                exp >>= 1;
            }
            return (seq_t)result;
        } else {                 /* odd n: floor( sqrt( n^n ) ) */
            idx_t exp_half = (n - 1) / 2;
            unsigned __int128 m      = 1;
            unsigned __int128 base   = (unsigned __int128)n;
            idx_t exp = exp_half;
            while (exp) {
                if (exp & 1ULL) m *= base;
                base *= base;
                exp >>= 1;
            }
            /* compute m^2 * n and take integer square root */
            unsigned __int128 prod = m * m * (unsigned __int128)n;
            unsigned long long root = sqrt_uint128(prod);
            return (seq_t)root;
        }
    }

    /* Big integer arithmetic for arbitrary‑precision builds */
    if ((n & 1ULL) == 0) {  /* even n */
        return big_pow((seq_t)n, (seq_t)(n / 2));
    } else {                /* odd n */
        seq_t m   = big_pow((seq_t)n, (seq_t)((n - 1) / 2));
        seq_t prod = seq_mul(seq_mul(m, m), (seq_t)n);
        return floor_sqrt(prod);
    }
}
