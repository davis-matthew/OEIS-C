/*
 * A051226  Numbers m such that the Bernoulli number B_m has denominator 30.
 * https://oeis.org/A051226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2*A051225(n)
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers m such that the Bernoulli number B_m has denominator 30. */
/* The algorithm tests each even multiple of 4 for the condition that the */
/* only primes p with (p‑1) | m are 2, 3 and 5.  The sequence is built */
/* lazily with a static cache so that subsequent calls are fast. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t modpow_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    __uint128_t result = 1;
    __uint128_t base   = a % mod;
    while (e)
    {
        if (e & 1U)
            result = (result * base) % mod;
        base = (base * base) % mod;
        e >>= 1U;
    }
    return (uint64_t)result;
}

/* Deterministic Miller–Rabin for 64‑bit integers using bases 2, 7, 61.
   Valid for all numbers < 2³², which covers the arguments we use (d+1 ≤ e+1). */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)                return false;
    if (n % 2 == 0)           return n == 2;
    if (n % 3 == 0)           return n == 3;

    uint64_t d = n - 1;
    int      s = 0;
    while ((d & 1U) == 0U) {
        d >>= 1U;
        ++s;
    }

    static const uint32_t bases[] = {2U, 7U, 61U};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint32_t a = bases[i];
        if (a >= n) continue;
        uint64_t x = modpow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (__uint128_t)x * x % n;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Checks whether an even multiple of 4 satisfies the denominator = 30 condition. */
static bool ok_e(uint64_t e)
{
    if (e % 4 != 0)
        return false;

    /* Enumerate all divisors d of e.  For each d > 1, if d+1 is a prime > 5,
       the Bernoulli denominator would contain that prime and be > 30. */
    for (uint64_t d = 1; d <= e / d; ++d) {
        if (e % d != 0) continue;
        uint64_t d1 = d;
        uint64_t d2 = e / d;

        if (d1 > 1 && d1 != 2 && d1 != 4) {
            uint64_t p = d1 + 1;
            if (p > 5 && is_prime_uint64(p))
                return false;
        }
        if (d2 != d1 && d2 > 1 && d2 != 2 && d2 != 4) {
            uint64_t p = d2 + 1;
            if (p > 5 && is_prime_uint64(p))
                return false;
        }
    }
    return true;
}

/* Public sequence function.  consecutive calls are served from a static cache. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n < 1, return 0 */

    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t next_candidate = 4;      /* first multiple of 4 */

    while (cache_len < (size_t)n) {
        /* Find the next valid term */
        while (true) {
            uint64_t e = next_candidate;
            next_candidate += 4;            /* next multiple of 4 */

            if (ok_e(e)) {
                /* Grow cache if needed */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 256;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) exit(1);        /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)e;
                break;                       /* next term stored */
            }
        }
    }

    return cache[(size_t)n - 1];
}
