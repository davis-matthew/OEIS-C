/*
 * A019418  Primes with primitive root 96.
 * https://oeis.org/A019418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 991 (2026-11-03)
 * Helpers:   factorize, is_prime_seq, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: Primes with primitive root 96
 *
 * a(n) is the n-th prime p for which 96 is a primitive root modulo p.
 * The algorithm searches successive primes and tests the primitive‑root
 * condition by checking 96^((p-1)/q) mod p ≠ 1 for every distinct prime
 * divisor q of p‑1.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factorize.h"
#include "../utils/is_prime_seq.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* Return the nth prime p such that 96 is a primitive root modulo p. */
seq_t a(idx_t n) {
    /* Static cache of found values. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Static state for continued search. */
    static uint64_t next_candidate = 5;     /* start from 5 (prime) */
    static uint64_t found = 0;              /* number of primes found */

    /* Grow cache when needed. */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Temporary arrays for factorization (distinct prime factors only). */
    uint64_t pf[64];
    int exps[64];

    /* Main search loop. */
    while (found < (size_t)n) {
        /* Skip even numbers (except 2), but start from 5 so only odds. */
        uint64_t p = next_candidate;

        /* Check primality. */
        if (is_prime_seq((seq_t)p) && p > 3) {
            /* 96 must be coprime to p; skip p dividing 96 (only 2,3). */
            if (p != 2 && p != 3) {
                /* Factor p-1. */
                uint64_t m = p - 1;
                size_t fc = factorize(m, pf, exps);

                /* Test primitive-root condition. */
                bool ok = true;
                for (size_t i = 0; i < fc; ++i) {
                    uint64_t q = pf[i];
                    seq_t res = powmod((seq_t)96, m / q, (seq_t)p);
                    if (res == (seq_t)1) {
                        ok = false;
                        break;
                    }
                }

                if (ok) {
                    /* Store found prime in cache. */
                    if (cache_len == cache_cap) {
                        /* Grow cache by a factor of 2. */
                        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                        ensure_capacity(new_cap, &cache_cap,
                                        OEISC_VPP(&cache), sizeof(seq_t));
                    }
                    cache[cache_len++] = (seq_t)p;
                    ++found;
                }
            }
        }

        /* Advance to next candidate prime (skip even numbers). */
        next_candidate += 2;
    }

    return cache[n - 1];
}
