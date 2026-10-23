/*
 * A184862  Primes of the form floor(n+nr-r/2), where r=(1+sqrt(5))/2.
 * https://oeis.org/A184862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime of the form floor(n + n*φ - φ/2), * with φ
 *            = (1+sqrt(5))/2 = golden ratio
 * Range:     u8 n <= 15, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include necessary standard headers for the utilities we use */
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Integer square‑root for 64‑bit values                              */
/* ------------------------------------------------------------------ */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t low = 0, high = x;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid <= x / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ------------------------------------------------------------------ */
/* Simple deterministic primality test for 64‑bit numbers.             */
/* ------------------------------------------------------------------ */
static bool is_prime_uint64(uint64_t v)
{
    if (v < 2)
        return false;
    if (v % 2 == 0)
        return v == 2;
    /* trial divide by odd numbers up to sqrt(v) */
    for (uint64_t d = 3; d <= v / d; d += 2)
        if (v % d == 0)
            return false;
    return true;
}

/* ------------------------------------------------------------------ */
/* Main function required by the framework.                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 by the OEIS definition ; we use it
     * as a 64‑bit unsigned value for arithmetic convenience.       */
    uint64_t N = (uint64_t)n;

    uint64_t found = 0;      /* number of primes found so far   */
    uint64_t m = 1;          /* current index used in the formula */
    seq_t result = 0;        /* the n‑th prime to return        */

    /* iterate m until we have collected N primes of the desired form */
    while (found < N) {
        /* compute k = 2*m - 1 (odd integer)                          */
        uint64_t k = 2 * m - 1;

        /* floor(k * φ) = floor((k + k*sqrt5) / 2)
         *   where floor(k*sqrt5) = floor(sqrt(5*k^2))                */
        uint64_t sq = isqrt_u64(5ULL * k * k);          /* floor(k*sqrt5) */
        uint64_t floor_k_phi = (k + sq) / 2;            /* floor(k*φ)    */

        /* compute the original expression:
         * floor(m + m*φ - φ/2) = floor((2*m + floor_k_phi) / 2)   */
        uint64_t val_u64 = (2 * m + floor_k_phi) / 2;
        seq_t   val      = (seq_t)val_u64;            /* cast to seq_t */

        /* primality test on val (value is small, fits in uint64_t) */
        if (is_prime_uint64(val_u64)) {
            ++found;
            if (found == N)
                result = val;
        }
        ++m;
    }

    return result;
}
