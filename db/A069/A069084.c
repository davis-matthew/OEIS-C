/*
 * A069084  Numbers n such that sigma(n)/tau(n) has denominator 5.
 * https://oeis.org/A069084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th integer m whose mean divisor σ(m)/τ(m) (σ = sum
 *            of divisors, τ = number of divisors) reduces to a fraction with
 *            denominator 5. The sequence is obtained by scanning consecutive
 *            integers, computing σ and τ via prime factorisation, reducing
 *            the fraction, and keeping those with denomi...
 * Range:     u8 n <= 8, u16 n <= 1734, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9144 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Standard headers – available both in C11 and C++17. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/*  Compute whether σ(n)/τ(n) has denominator 5.
    σ and τ are rationally reduced.  For n≤2^64 this is safe. */
static int has_denom_five(uint64_t n)
{
    unsigned __int128 sigma = 1;   /* product of partial sums */
    uint64_t tau = 1;              /* product of (exponent+1) */

    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }

            /* σ contribution = (p^(e+1)-1)/(p-1)  */
            unsigned __int128 sum = 1;
            unsigned __int128 pow = 1;
            for (uint64_t i = 1; i <= e; ++i) {
                pow *= p;
                sum += pow;
            }
            sigma *= sum;
            tau *= (e + 1);
        }
    }

    if (m > 1) {                    /* remaining prime factor */
        unsigned __int128 sum = (unsigned __int128)m + 1;
        sigma *= sum;
        tau *= 2;
    }

    /* Compute gcd(sigma, tau).  sigma is 128‑bit, tau fits in 64‑bit. */
    unsigned __int128 a = sigma;
    uint64_t b = tau;
    while (b) {
        unsigned __int128 r = a % b;
        a = b;
        b = (uint64_t)r;
    }

    uint64_t denom = tau / (uint64_t)a;  /* tau / gcd */
    return denom == 5;
}

/*  Cache for the sequence so that repeated calls are efficient.  */
static seq_t *cache = NULL;       /* array of a(1), a(2), … */
static size_t cache_len  = 0;     /* number of stored terms */
static size_t cache_cap  = 0;     /* allocated capacity */
static uint64_t next_candidate = 1;  /* next natural number to test */

/*  Ensure cache is large enough to hold 'need' terms.  */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_arr = mem_resize(seq_t, cache, new_cap);
    cache = new_arr;
    cache_cap = new_cap;
}

/*  Main sequence generator.  */
seq_t a(idx_t n)
{
    /*  The sequence is 1‑based.  */
    if (n == 0) return 0;               /* undefined, but harmless */

    while (cache_len < (size_t)n) {
        ensure_cache(cache_len + 1);
        if (has_denom_five(next_candidate)) {
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
