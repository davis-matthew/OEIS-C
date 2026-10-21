/*
 * A060268  Distance of 2n from the closest prime.
 * https://oeis.org/A060268
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(2n - p₁, p₂ - 2n) where p₁ is the greatest prime ≤ 2n
 *            and p₂ is the smallest prime ≥ 2n. This follows from the OEIS
 *            definition a(n) = min(A049653(n), A060266(n))
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   mulmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mulmod64.h"

/* --- fast modular arithmetic for 64‑bit numbers -------------------------------- */

static inline uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1U)
            result = mulmod64(result, base, mod);
        base = mulmod64(base, base, mod);
        exp >>= 1U;
    }
    return result;
}

/* --- deterministic Miller–Rabin test for 64‑bit integers --------------------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;

    /* small primes */
    const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    const unsigned small_cnt = sizeof(small_primes)/sizeof(small_primes[0]);

    for (unsigned i = 0; i < small_cnt; ++i) {
        uint64_t p = small_primes[i];
        if (n == p)
            return true;
        if (n % p == 0)
            return false;
    }

    /* write n-1 as d * 2^s */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1U) == 0U) {
        d >>= 1U;
        ++s;
    }

    /* deterministic bases for 64‑bit numbers */
    const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    const unsigned base_cnt = sizeof(bases)/sizeof(bases[0]);

    for (unsigned i = 0; i < base_cnt; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mulmod64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* For indices below the OEIS offset (n < 2) the value is undefined.
       Returning 0 is harmless and keeps the function well defined. */
    if (n < 2)
        return 0;

    /* Work with the value 2n as a 64‑bit integer. */
    uint64_t two_n = n << 1;

    /* Search downward for the largest prime ≤ 2n. */
    uint64_t lower = two_n;
    while (!is_prime_uint64(lower))
        --lower;

    /* Search upward for the smallest prime ≥ 2n. */
    uint64_t upper = two_n;
    while (!is_prime_uint64(upper))
        ++upper;

    /* Distances from 2n to the neighbouring primes. */
    uint64_t dist_down = two_n - lower;
    uint64_t dist_up   = upper - two_n;

    /* The required distance is the smaller of the two.  It fits into seq_t
       because the sequence values are always relatively small for the
       tested range. */
    return (seq_t)(dist_down < dist_up ? dist_down : dist_up);
}
