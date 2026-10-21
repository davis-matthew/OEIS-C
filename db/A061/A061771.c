/*
 * A061771  Primes p(k) such that p(k+1) - p(k) = 2^m for some m (smaller of a
 *          pair of consecutive primes which differ by a power of 2).
 * https://oeis.org/A061771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A061771 – primes p such that the next prime p_next satisfies
 *            p_next – p = 2^m for some integer m > 0. The function generates
 *            successive primes and records those whose gap to the next prime
 *            is a power of two. It caches the generated terms so subsequent
 *            calls (in any order) can be answered quickly
 * Range:     u8 n <= 34, u16 n <= 2504, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ---------- 64‑bit deterministic Miller–Rabin primality test ---------- */
static inline uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t modpow(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    while (d)
    {
        if (d & 1) r = mulmod(r, a, mod);
        a = mulmod(a, a, mod);
        d >>= 1;
    }
    return r;
}

static int is_prime_64(uint64_t n)
{
    if (n < 2) return 0;
    static const uint64_t small[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small) / sizeof(small[0]); ++i)
    {
        uint64_t p = small[i];
        if (n == p) return 1;
        if (n % p == 0) return 0;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        int ok = 0;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mulmod(x, x, n);
            if (x == n - 1) { ok = 1; break; }
        }
        if (!ok) return 0;
    }
    return 1;
}

/* ---------- Reallocation helper that works in both C and C++ ---------- */
#ifdef __cplusplus
#define RELOC(p, sz) reinterpret_cast<seq_t*>(realloc((p),(sz)))
#else
#define RELOC(p, sz) (seq_t*)realloc((p),(sz))
#endif

/* ----------  Sequence generator ---------- */
seq_t a(idx_t n)
{
    static seq_t *results = NULL;
    static idx_t result_cnt = 0;
    static size_t result_cap = 0;

    static idx_t last_checked_prime = 1;      /* prime just processed */
    static idx_t next_candidate = 3;          /* next value to test */
    static uint64_t previous_prime = 2;       /* last prime found */

    if (n == 0) return (seq_t)0;              /* undefined for n < 1 */

    /* Extend the pre‑computed list until we have n terms */
    while (result_cnt < n)
    {
        /* Find the next prime after last_checked_prime */
        uint64_t cand = next_candidate;
        if (cand <= 2) cand = 3;
        else if (cand == 3) {}              /* keep 3 */
        /* Continue searching if not prime */
        while (!is_prime_64(cand))
            cand += (cand == 2) ? 1 : 2;    /* skip even numbers */

        /* Compute gap to previous prime */
        uint64_t gap = cand - previous_prime;

        /* Check if gap is a power of two (gap > 0) */
        if ((gap > 0) && ((gap & (gap - 1)) == 0))
        {
            /* Grow array if needed */
            if (result_cnt == result_cap)
            {
                size_t new_cap = result_cap ? result_cap * 2 : 16;
                seq_t *tmp = RELOC(results, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* out of memory */
                results = tmp;
                result_cap = new_cap;
            }
            results[result_cnt++] = (seq_t)previous_prime;
        }

        /* Advance to next prime */
        previous_prime = cand;
        last_checked_prime = cand;
        next_candidate = cand + 2;
    }

    return results[n - 1];
}
