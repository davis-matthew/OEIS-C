/*
 * A099746  Primes arising in A099744.
 * https://oeis.org/A099746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the n‑th prime of the form 1 0 M 0 1, * where M is an
 *            odd‑length palindrome and the whole number is prime. * Generated
 *            by a simple incremental search
 * Range:     u8 none, u16 n <= 3, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-03)
 * Helpers:   ipow_idx, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <limits.h>
#include "../utils/ipow_idx.h"
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

/* build palindrome M from the first half (idx_t)   */
static inline seq_t make_pal_from_half(idx_t half, int half_len)
{
    seq_t pal = (seq_t)half;
    idx_t rev = half / 10;          /* skip middle digit when mirroring */
    while (rev > 0) {
        pal = pal * 10 + (rev % 10);
        rev /= 10;
    }
    return pal;
}

/* small pow10 helper for idx_t (used to compute half limits) */
static inline idx_t pow10_u64(int e)
{
    idx_t r = 1;
    while (e-- > 0)
        r *= 10;
    return r;
}

/* Main sequence function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    /* 1‑indexed sequence, n == 0 is undefined */
    if (n == 0) return (seq_t)0;

    /* static cache for memoisation                                                 */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    /* state for the incremental generator                                         */
    static int    cur_l       = 0;      /* current odd length of M              */
    static int    half_len    = 0;      /* (cur_l+1)/2                          */
    static idx_t  cur_half    = 0;      /* next half to try                      */
    static idx_t  half_end    = 0;      /* last half for the current length      */
    static seq_t  base        = 0;      /* 10^(cur_l+3)                          */

    /* initialise generator on first call                                      */
    if (cur_l == 0) {
        cur_l = 1;                     /* start with length 1 (M of 1 digit)  */
        half_len = (cur_l + 1) / 2;    /* 1                                 */
        cur_half = pow10_u64(half_len - 1); /* 1 */
        half_end = pow10_u64(half_len) - 1; /* 9 */
        base = ipow_idx((seq_t)10, (idx_t)(cur_l + 3));
    }

    /* generate primes until we have at least n terms                         */
    while (cache_len < n) {
        while (cur_half <= half_end && cache_len < n) {
            seq_t M = make_pal_from_half(cur_half, half_len);
            seq_t candidate = base + M * 100 + 1;   /* 1 0 M 0 1                */
            if (isPrimeGeneric(candidate)) {
                ensure_capacity(cache_len + 1, &cache_cap, OEISC_VPP(&cache),
                                sizeof(seq_t));
                cache[cache_len++] = candidate;
            }
            ++cur_half;
        }

        /* move to next odd length if we finished the current one              */
        if (cur_half > half_end) {
            cur_l += 2;
            half_len = (cur_l + 1) / 2;
            cur_half = pow10_u64(half_len - 1);
            half_end = pow10_u64(half_len) - 1;
            base = ipow_idx((seq_t)10, (idx_t)(cur_l + 3));
        }
    }

    return cache[n - 1];   /* 1‑based indexing                              */
}
