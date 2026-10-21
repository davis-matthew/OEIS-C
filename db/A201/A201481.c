/*
 * A201481  Primes of the form 5*k^2 + 2, k >= 0.
 * https://oeis.org/A201481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 20, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS sequence A201481: primes of the form 5*k^2 + 2, for k ≥ 0.
 * a(1) = 2, a(2) = 7, a(3) = 47, …
 * The function generates each term on demand, memoising the sequence. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_big.h"   /* for arbitrary‑precision builds */

static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2)            return false;
    if (n % 2 == 0)       return n == 2;
    if (n % 3 == 0)       return n == 3;
    /* Miller–Rabin deterministic for 64‑bit integers */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }

    /* fixed bases sufficient for 64‑bit */
    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
                                     19ULL, 23ULL, 29ULL, 31ULL, 37ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        __uint128_t x = 1, pow = a, exp = d;
        while (exp)
        {
            if (exp & 1) x = x * pow % n;
            pow = pow * pow % n;
            exp >>= 1;
        }
        uint64_t xx = (uint64_t)x;
        if (xx == 1 || xx == n - 1) continue;

        bool comp = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = (__uint128_t)x * x % n;
            xx = (uint64_t)x;
            if (xx == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache       = NULL;    /* array of found primes            */
    static size_t  count      = 0;       /* number of primes stored so far   */
    static size_t  capacity   = 0;       /* allocated size of cache          */
    static uint64_t next_k    = 0;       /* next k to test (k=0 for first)   */

    /* On the first call initialise cache capacity */
    if (capacity == 0) {
        capacity = 16;
        cache = (seq_t*)realloc(NULL, capacity * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed – return sentinel */
    }

    /* Keep generating until we have at least n primes */
    while (count < n) {
        uint64_t k = next_k;

        /* candidate = 5*k^2 + 2 */
        __uint128_t prod = (__uint128_t)5 * k * k + 2;   /* fits 128‑bit for k up to ~2^63 */

        seq_t val = (seq_t)prod;  /* cast; may truncate for small seq_t types */

        /* Decide primality test based on seq_t size */
        bool prime;
        if (sizeof(seq_t) == sizeof(uint64_t))
            prime = is_prime_uint64((uint64_t)val);
        else
            prime = is_prime_big(val);

        if (prime) {
            if (count == capacity) {
                size_t newcap = capacity * 2;
                seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) { free(cache); cache = NULL; capacity = 0; return 0; }
                cache = tmp;
                capacity = newcap;
            }
            cache[count++] = val;
        }
        ++next_k;
    }

    return cache[n - 1];
}
