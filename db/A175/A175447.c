/*
 * A175447  a(n)>a(n-1), a(n) = smallest prime such that a(n)+a(n-1) is
 *          multiple of m, a(1)=2, m=13.
 * https://oeis.org/A175447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime > a(n-1) such that a(n)+a(n-1) ≡ 0 (mod
 *            13); a(1)=2
 * Range:     u8 n <= 5, u16 n <= 658, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 947 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t len    = 0;          /* last computed index (1‑based) */

    /* Return cached value if already computed */
    if (n <= len)
        return cache[n];

    /* Make sure the array can hold indices up to n */
    ensure_capacity((size_t)n + 1, &cap, OEISC_VPP(&cache), sizeof(seq_t));

    /* Initialise the sequence if it hasn't been done yet */
    if (len == 0) {
        cache[1] = (seq_t)2;
        len = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t k = len + 1; k <= n; ++k) {
        seq_t prev = cache[k - 1];

        /* Desired residue for the next prime: p + prev ≡ 0 (mod 13) */
        int prev_mod    = (int)(prev % 13);
        int target_mod  = (13 - prev_mod) % 13;

        /* Start from the next integer after prev, adjusted to the target residue */
        seq_t cand = prev + 1;
        int cand_mod = (int)(cand % 13);
        int delta = (target_mod - cand_mod + 13) % 13;
        cand = cand + delta;

        /* Find the first prime in this residue class */
        while (!isPrimeGeneric(cand))
            cand = cand + 13;

        cache[k] = cand;
    }

    len = n;
    return cache[n];
}
