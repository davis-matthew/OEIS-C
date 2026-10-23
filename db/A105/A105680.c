/*
 * A105680  Numbers k such that 1009*k + 9973 is prime.
 * https://oeis.org/A105680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 3883, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n‑th integer k such that 1009*k + 9973 is prime. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Deterministic Miller–Rabin for 64‑bit integers. */
static uint64_t modmul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t modpow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1) r = modmul64(r, a, mod);
        a = modmul64(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool is_prime64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes */
    const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
    {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }

    /* Miller–Rabin with bases {2,3,5,7,11,13} is deterministic
       for 64‑bit integers. */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    const uint64_t bases[] = {2, 3, 5, 7, 11, 13};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i];
        if (a % n == 0) continue; /* skip if a == n */
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r)
        {
            x = modmul64(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* Cached list of k values */
static seq_t   *k_cache   = NULL;
static size_t  k_size     = 0;
static size_t  k_capacity = 0;
static uint64_t next_k = 0;          /* next even k to test */

/* Resize cache and extend it until we have `required` elements. */
static void ensure_cache(size_t required)
{
    if (k_size >= required) return;

    /* Prepare capacity */
    size_t cap = k_capacity ? k_capacity : 128;
    while (cap < required) cap <<= 1;
    if (cap != k_capacity) {
        seq_t *ptr = (seq_t *)realloc(k_cache, cap * sizeof(seq_t));
        if (!ptr) abort();          /* should never happen */
        k_cache   = ptr;
        k_capacity = cap;
    }

    /* Generate new k values until we have `required` of them */
    while (k_size < required) {
        /* Ensure we test only even k (odd gives an even number, composite). */
        if ((next_k & 1ULL) != 0) next_k++;

        uint64_t cand = 1009ULL * next_k + 9973ULL;
        if (is_prime64(cand))
            k_cache[k_size++] = (seq_t)next_k;

        next_k += 2;                     /* advance to next even k */
    }
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out of range, behaviour unspecified */
    ensure_cache((size_t)n);
    return k_cache[n-1];
}
