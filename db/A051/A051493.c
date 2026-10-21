/*
 * A051493  Triangles with perimeter n and relatively prime integer side
 *          lengths.
 * https://oeis.org/A051493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d)·A005044(n/d), where A005044(m) =
 *            round(m²/48) for even m, round((m+3)²/48) for odd m
 * Range:     u8 n <= 108, u16 n <= 1776, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gen_divs_with_mu, triangles_of_perimeter
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gen_divs_with_mu.h"
#include "../utils/triangles_of_perimeter.h"

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Factor `n`.  Use at most 16 distinct primes for n ≤ 10⁴. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t pcnt = 0;
    uint64_t temp = (uint64_t)n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pcnt]   = p;
            exps[pcnt]     = e;
            ++pcnt;
        }
    }
    if (temp > 1) {            /* remaining prime factor */
        primes[pcnt] = temp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* Generate all divisors and their Möbius μ value. */
    uint64_t divs[128];
    int       mu_vals[128];
    size_t    dcnt = 0;
    gen_divs_with_mu(primes, exps, pcnt,
                     0, 1ULL, 1, 0,
                     divs, mu_vals, &dcnt);

    /* Sum over divisors: μ(d) * A005044(n/d). */
    long long sum = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divs[i];
        int mu = mu_vals[i];
        if (mu == 0)
            continue;
        idx_t k = n / d;
        seq_t t = triangles_of_perimeter(k);
        if (mu > 0)
            sum += (long long)t;
        else
            sum -= (long long)t;
    }
    return (seq_t)sum;
}
