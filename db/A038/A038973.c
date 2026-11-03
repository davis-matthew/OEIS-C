/*
 * A038973  Primes p such that 86 is a square mod p.
 * https://oeis.org/A038973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that 86 is a quadratic residue *
 *            modulo p. This includes the primes dividing 86 (p = 2 and 43) *
 *            because 0 ≡ 0² mod p is a square
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 650 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t, NULL */
#include <stdbool.h>  /* bool */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* NULL, size_t */

#include "../utils/is_prime.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
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

/* Cached list of primes p with 86 a quadratic residue mod p.  The list
 * is built lazily; extend_cache will fill it until it contains at
 * least the requested number of elements. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of cached primes */
static size_t cache_cap  = 0;   /* allocated capacity of the array */
static uint64_t next_start = 2; /* next candidate to start searching from */

/* Extend the cache until it has at least 'target' primes. */
static void extend_cache(size_t target)
{
    while (cache_len < target) {
        uint64_t cand = next_start;

        /* Start at 2 if not yet used; otherwise ensure an odd candidate. */
        if (cand <= 2)
            cand = 2;
        else if ((cand & 1ULL) == 0)
            cand++;

        /* Search for the next prime that satisfies the quadratic residue
         * condition for 86.  A prime p divides 86 iff 86 % p == 0, and
         * in that case 86 is clearly a square (0²) modulo p.  For other
         * primes we require the Legendre (Jacobi) symbol (86|p) to be
         * +1. */
        for (;;) {
            if (is_prime((idx_t)cand)) {
                bool ok = (jacobi_symbol(86ULL, cand) == 1) || (86ULL % cand == 0);
                if (ok) {
                    /* Grow the cache if necessary. */
                    if (cache_len == cache_cap) {
                        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                        ensure_capacity(cache_len + 1, &cache_cap,
                                        OEISC_VPP(&cache), sizeof(seq_t));
                    }
                    cache[cache_len++] = (seq_t)cand;
                    /* Next candidate belongs to the next odd number. */
                    next_start = cand + 2;
                    break;
                }
            }
            cand += 2;  /* only odd candidates beyond 2 */
        }
    }
}

/* Return the nth term of the sequence (a(1)=2).  Undefined for n = 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* n = 0 is outside the defined range */
    if (cache_len < (size_t)n)
        extend_cache((size_t)n);
    return cache[(size_t)n - 1];
}
