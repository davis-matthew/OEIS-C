/*
 * A076830  Least square s such that A078142(s) is equal to the n-th prime.
 * https://oeis.org/A076830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m^2 where m is the least integer with A078142(m^2) =
 *            prime(n), i.e. sum over distinct primes q | m of ((floor(sqrt
 *            q)+1)^2 - q) = prime(n). Computed by a sieve of that cost sum
 *            f(m), then a scan for the least m; the scan bound is the least
 *            prime t^2 - p, with cost p, from the single-prime...
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 93, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 46 terms through n = 972 (2026-11-07)
 * Helpers:   is_prime_u64, nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/nth_prime.h"
#include "../utils/is_prime_u64.h"

static uint32_t *A076830_f = 0;
static uint64_t A076830_lim = 0;

static uint64_t A076830_isqrt(uint64_t v)
{
    uint64_t r = (uint64_t)sqrt((double)v);
    while (r * r > v) r--;
    while ((r + 1) * (r + 1) <= v) r++;
    return r;
}

static void A076830_build(uint64_t lim)
{
    if (A076830_f) free(A076830_f);
    A076830_f = (uint32_t *)calloc((size_t)(lim + 1), sizeof(uint32_t));
    A076830_lim = lim;
    for (uint64_t q = 2; q <= lim; q++) {
        if (A076830_f[q] == 0) {
            uint64_t t = A076830_isqrt(q) + 1;
            uint32_t c = (uint32_t)(t * t - q);
            for (uint64_t k = q; k <= lim; k += q) A076830_f[k] += c;
        }
    }
}

seq_t a(idx_t n)
{
    uint64_t p = nth_prime(n);
    uint64_t t = p / 2;
    if (t < 2) t = 2;
    uint64_t U = 0;
    for (;; t++) {
        if (2 * t - 1 <= p) continue;
        uint64_t q = t * t - p;
        if (q >= 2 && is_prime_u64(q)) { U = q; break; }
    }
    if (U > A076830_lim) A076830_build(U);
    uint64_t m = 2;
    for (; m <= A076830_lim; m++) {
        if (A076830_f[m] == p) break;
    }
    return (seq_t)m * (seq_t)m;
}
