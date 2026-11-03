/*
 * A161402  Numbers having more than 2 primes among the permutations of their
 *          digits.
 * https://oeis.org/A161402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A161402: numbers having more than 2 primes among
   permutations of their decimal digits (leading zeros ignored). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Recursive helper to count primes among digit permutations */
static void permute_rec(int pos, int len, int counts[10],
                        unsigned long long current, int *prime_cnt)
{
    if (*prime_cnt > 2) return;
    if (pos == len) {
        if (is_prime_big((seq_t)current)) (*prime_cnt)++;
        return;
    }
    for (int d = 0; d < 10; ++d) {
        if (counts[d] > 0) {
            --counts[d];
            permute_rec(pos + 1, len, counts, current * 10 + d, prime_cnt);
            ++counts[d];
            if (*prime_cnt > 2) return;
        }
    }
}

/* Determine whether n has more than two prime permutations of its digits */
static bool qualifies(seq_t n)
{
    /* Extract digits and counts */
    unsigned long long val = (unsigned long long)n;
    int counts[10] = {0};
    int len = 0;
    if (val == 0) return false;
    while (val > 0) {
        ++counts[val % 10];
        val /= 10;
        ++len;
    }

    int prime_cnt = 0;
    permute_rec(0, len, counts, 0ULL, &prime_cnt);
    return prime_cnt > 2;
}

/* Static cache for computed sequence terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;
static idx_t max_checked = 99;   /* last integer tested */

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but safe return */

    /* initialize cache on first call */
    if (cache == NULL) {
        ensure_capacity(1024, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache_size = 0;
        max_checked = 99;
    }

    /* generate terms until we have n items */
    seq_t cand = (seq_t)(max_checked + 1);
    while (cache_size < n) {
        if (qualifies(cand)) {
            ensure_capacity(cache_size + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
            cache[cache_size++] = cand;
        }
        ++cand;
    }
    max_checked = (idx_t)cand - 1;

    return cache[n - 1];
}
