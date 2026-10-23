/*
 * A108061  Numbers k such that 193*k + 191 is prime.
 * https://oeis.org/A108061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 4275, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th nonnegative integer k such that 193*k + 191 is prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* 64‑bit multiplication modulo m using 128‑bit intermediate. */
static inline uint64_t mulmod64(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)(((__uint128_t)a * b) % m);
}

/* 64‑bit modular exponentiation. */
static uint64_t powmod64(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    a %= m;
    while (e > 0)
    {
        if (e & 1ULL)
            r = mulmod64(r, a, m);
        a = mulmod64(a, a, m);
        e >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin for 64‑bit integers. */
static bool isPrime64(uint64_t n)
{
    if (n < 2)
        return false;

    /* Small prime checks */
    static const uint64_t small_primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n % p == 0)
            return n == p;
    }

    /* Write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL)
    {
        d >>= 1;
        ++s;
    }

    /* Bases sufficient for deterministic test up to 2^64. */
    static const uint64_t bases[] = {
        2ULL,
        325ULL,
        9375ULL,
        28178ULL,
        450775ULL,
        9780504ULL,
        1795265022ULL
    };

    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;

        uint64_t x = powmod64(a, d, n);
        if (x == 1ULL || x == n - 1)
            continue;

        bool cont = false;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mulmod64(x, x, n);
            if (x == n - 1)
            {
                cont = true;
                break;
            }
        }
        if (!cont)
            return false;
    }
    return true;
}

/* Dynamic array of found k‑values. */
static seq_t *seq = NULL;
static idx_t seq_size = 0;
static idx_t seq_cap = 0;
static uint64_t next_k = 0;        /* next even k to test */

/* Ensure capacity for at least one more element. */
static void seq_push(seq_t val)
{
    if (seq_size == seq_cap)
    {
        size_t new_cap = seq_cap ? (size_t)seq_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq = tmp;
        seq_cap = (idx_t)new_cap;
    }
    seq[seq_size++] = val;
}

/* Build the array up to at least n elements. */
static void seq_ensure_size(idx_t n)
{
    while (seq_size < n)
    {
        /* Only even k can give an odd prime. */
        if ((next_k & 1ULL) == 1ULL)
            ++next_k;                            /* make next_k even */

        uint64_t candidate = 193ULL * next_k + 191ULL;
        if (isPrime64(candidate))
            seq_push((seq_t)next_k);

        next_k += 2ULL;                         /* advance to next even k */
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                      /* undefined for n < 1 */

    seq_ensure_size(n);
    return seq[n - 1];
}
