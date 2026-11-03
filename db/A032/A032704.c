/*
 * A032704  Numbers k such that k prefixed by '4' and followed by '1' is
 *          prime.
 * https://oeis.org/A032704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 269, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 868 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that the integer 4k1 is prime.
   Returns the n-th such k, with a(1)=0. */
/* Build by iterating k and testing primality of 4*10^(digits(k)+1)+k*10+1
   Caching results so that non‑sequential calls are efficient. */

#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits0.h"
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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

seq_t a(idx_t n) {
    /* static cache: all found k values */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated capacity */
    static size_t used = 0;         /* number of k's stored */
    static seq_t next_k = 0;        /* next candidate k to test */

    /* Grow cache until we have enough entries */
    while (used < n) {
        /* count digits of next_k (uint64_t is enough for required n) */
        int d = num_digits0((uint64_t)next_k);
        /* construct N = 4*10^(d+1) + next_k*10 + 1 */
        seq_t pow = pow10((idx_t)(d + 1));
        seq_t N = (seq_t)4 * pow + (seq_t)next_k * (seq_t)10 + (seq_t)1;
        /* test primality */
        if (isPrimeGeneric(N)) {
            /* ensure capacity for next element */
            if (used + 1 > cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                ensure_capacity(new_cap, &cap, OEISC_VPP(&cache), sizeof(seq_t));
            }
            cache[used++] = next_k;
        }
        next_k++;
    }
    return cache[n - 1];
}
