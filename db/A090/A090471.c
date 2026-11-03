/*
 * A090471  Smallest prime not already used that is of the form prime(n)+kn
 *          for some k > 0.
 * https://oeis.org/A090471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest unused prime of the form prime(n) + k*n, k>0
 * Range:     u8 n <= 22, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A090471 – grows lazily and is cache‑aware. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

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

/* Cache of computed a(n) values (0‑based).  cached_n holds the last
   index that was cached (1‑based). */
static idx_t   cached_n = 0;
static seq_t*  cache    = NULL;
static size_t  cache_cap= 0;

/* List of primes that have already appeared in the sequence.
   All comparisons are done via linear search – sufficient for
   n up to 10⁴.  Each element is a prime that has been output. */
static seq_t*  used     = NULL;
static size_t  used_len = 0;
static size_t  used_cap = 0;

/* Resize an array of size 'elem_size' (in bytes) up to at least
   'need' elements, using power‑of‑two growth. */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t need, size_t *cap, T **ptr, size_t elem_size)
{
    if (*cap < need) {
        size_t new_cap = (*cap > 0) ? (*cap << 1) : 16;
        if (new_cap < need) new_cap = need;
        *ptr = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
        *cap = new_cap;
    }
}
#else
static void ensure_capacity(size_t need, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap < need) {
        size_t new_cap = (*cap > 0) ? (*cap << 1) : 16;
        if (new_cap < need) new_cap = need;
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
}
#endif

/* Linear membership test.  For big seq_t this still works
   because the == operator is defined. */
static bool used_contains(seq_t val)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used[i] == val) return true;
    return false;
}

static void used_add(seq_t val)
{
    if (used_cap == used_len)
        ensure_capacity(used_len + 1, &used_cap, OEISC_VPP(&used), sizeof(seq_t));
    used[used_len++] = val;
}

/* Compute the nth term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Extend the cache if necessary. */
    if (n > cached_n) {
        /* Allocate space for new terms. */
        ensure_capacity(n, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));

        for (idx_t i = cached_n + 1; i <= n; ++i) {
            uint32_t p_i = get_prime(i);                /* i‑th prime, i is 1‑based */

            /* Search for the smallest prime not yet used. */
            idx_t k = 1;
            while (1) {
                seq_t candidate = (seq_t)p_i + ((seq_t)k * (seq_t)i);
                if (is_prime_big(candidate) && !used_contains(candidate)) {
                    used_add(candidate);
                    cache[i - 1] = candidate;           /* cache is 0‑based */
                    break;
                }
                ++k;
            }
        }
        cached_n = n;
    }
    return cache[n - 1];
}
