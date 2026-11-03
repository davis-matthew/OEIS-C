/*
 * A186782  Primes of the form k*6^m-1 with k prime and m an integer.
 * https://oeis.org/A186782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form k*6^m - 1 with k prime and m ≥ 0
   (OEIS A186782).                                          */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
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
    /* Static cache of the sequence terms */
    static seq_t *cache = NULL;
    static size_t cached = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 0;  /* candidate to start searching from */

    if (n == 0) return (seq_t)0; /* unspecified for offset zero */

    /* Generate terms up to the required index */
    while (cached < n) {
        /* Start searching from the next candidate value */
        seq_t candidate;
        if (next_candidate < 2)
            candidate = 2;
        else
            candidate = next_candidate;

        /* Find the next prime candidate */
        do {
            if (!is_prime_big(candidate))
                candidate += (candidate == 2 ? 1 : 2); /* skip evens after 2 */
        } while (!is_prime_big(candidate));

        /* Update next_candidate for subsequent searches */
        next_candidate = candidate + ((candidate == 2) ? 1 : 2);

        /* Test whether candidate satisfies the form k*6^m - 1 */
        seq_t t = candidate + 1;               /* t = p + 1 */
        idx_t v2 = 0, v3 = 0;
        seq_t tmp = t;

        /* Count powers of 2 */
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            v2++;
        }
        /* Count powers of 3 */
        while (tmp % 3 == 0) {
            tmp /= 3;
            v3++;
        }
        idx_t m = (v2 < v3) ? v2 : v3;        /* exponent of 6 */

        /* Compute 6^m as the divisor */
        seq_t denom = 1;
        for (idx_t i = 0; i < m; ++i)
            denom *= 6;

        /* k = (candidate + 1) / 6^m */
        seq_t k = (candidate + 1) / denom;

        if (is_prime_big(k)) {
            /* Ensure cache capacity and store the term */
            ensure_capacity(cached + 1, &cache_cap, OEISC_VPP(&cache),
                            sizeof(seq_t));
            cache[cached++] = candidate;
        }
    }

    return cache[n - 1];
}
