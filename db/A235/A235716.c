/*
 * A235716  Average of twin prime pairs n having their decimal expansion of
 *          the form abcabc or abcabc0 such that n contains three twin primes
 *          as divisors.
 * https://oeis.org/A235716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: full, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A235716: n = 30030 * k  with  k = 1,2,3,…  and
   both  n – 1  and  n + 1  prime.  The terms are generated
   in increasing order of k and cached for repeated calls. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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

static seq_t *cache   = NULL;   /* cached terms   */
static size_t cache_len = 0;    /* number of terms stored */
static size_t cache_cap = 0;    /* capacity of the cache   */
static idx_t  next_k   = 1;     /* k value to test next     */

/* Append a new term, enlarging the cache if necessary. */
static void append_term(seq_t val)
{
    if (cache_len == 0 || val != cache[cache_len-1]) {   /* avoid duplicates */
        ensure_capacity(cache_len + 1, &cache_cap,
                        OEISC_VPP(&cache), sizeof(seq_t));
        cache[cache_len++] = val;
    }
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for n < 1 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* current candidate */
        seq_t val = (seq_t)30030 * (seq_t)next_k;   /* val = 30030·k */

        /* test the twin‑prime condition */
        if (is_prime_big(val - 1) && is_prime_big(val + 1))
            append_term(val);

        ++next_k;
    }

    return cache[n-1];    /* zero‑based indexing into the cache */
}
