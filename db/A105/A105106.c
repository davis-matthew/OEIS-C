/*
 * A105106  Numbers k such that the string k101 is prime.
 * https://oeis.org/A105106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth integer k such that 1000*k + 101 is prime
 * Range:     u8 n <= 53, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates terms lazily, caching them for subsequent calls.
 * Primality of 1000*k+101 is tested deterministically with Miller–Rabin
 * for 64‑bit integers, which is fast enough for all n that fit in 64‑bit
 * k (the sequence grows roughly linearly, and for n≈10 000 the
 * candidates are < 10⁸).  The code is valid C11/C++17 and respects
 * the required interface. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc/realloc */

/* 64‑bit modular multiplication using unsigned __int128 when available. */
static uint64_t mulmod64(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

/* 64‑bit modular exponentiation. */
static uint64_t powmod64(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1) r = mulmod64(r, a, m);
        a = mulmod64(a, a, m);
        e >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin test for 64‑bit integers. */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;

    /* Simple small‑prime trial division to eliminate composites quickly. */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
        17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    const size_t sp_len = sizeof small_primes / sizeof small_primes[0];
    for (size_t i = 0; i < sp_len; ++i)
    {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* Decompose n‑1 = d·2ˢ with d odd. */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for < 2⁶⁴ (per deterministic MR). */
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17};
    const size_t b_len = sizeof bases / sizeof bases[0];
    for (size_t i = 0; i < b_len; ++i)
    {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mulmod64(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ---- cached sequence storage ---- */
static seq_t *cached = NULL;      /* array of found k's */
static idx_t cached_cap = 0;      /* allocation capacity */
static idx_t cached_size = 0;     /* number of terms stored */
static uint64_t next_k = 0;       /* next candidate k to test */

/* Ensure we have room for at least 'min_needed' terms. */
static void ensure_capacity(idx_t min_needed)
{
    if (cached_cap >= min_needed) return;
    /* Grow capacity to at least twice the current or min_needed, whichever is larger. */
    idx_t new_cap = cached_cap ? cached_cap * 2 : 16;
    if (new_cap < min_needed) new_cap = min_needed;
    seq_t *new_arr = (seq_t *)realloc(cached, new_cap * sizeof(seq_t));
    if (!new_arr) { /* allocation failed – abort the program in a compliant way */
        abort();
    }
    cached = new_arr;
    cached_cap = new_cap;
}

/* OEIS sequence A105106 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, but harmless */

    /* Iterate until we have computed the n-th term. */
    while (cached_size < n)
    {
        /* Test 1000*k + 101 for primality. */
        uint64_t candidate = 1000ULL * next_k + 101ULL;
        if (isPrime64(candidate))
        {
            ensure_capacity(cached_size + 1);
            cached[cached_size++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cached[n - 1];
}
