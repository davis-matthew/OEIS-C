/*
 * A157478  a(n) is the least prime p such that p is greater than any previous
 *          term and is representable as the arithmetic mean of two other
 *          primes in exactly n different ways.
 * https://oeis.org/A157478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = least prime p > all previous terms that is the arithmetic mean of
   two other primes in exactly n distinct ways. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

seq_t a(idx_t n) {
    /* Static storage for memoisation */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;

    static uint64_t *prime_list = NULL;
    static size_t prime_cap = 0;
    static size_t prime_cnt = 0;
    static uint64_t last_prime = 3; /* last prime processed in prime_list */

    /* Initialise prime list with the first two primes */
    if (prime_cnt == 0) {
        ensure_capacity(2, &prime_cap, OEISC_VPP(&prime_list), sizeof(uint64_t));
        prime_list[0] = 2;
        prime_list[1] = 3;
        prime_cnt = 2;
        last_prime = 3;
    }

    /* Build the sequence up to the requested index n */
    while (terms_len < n) {
        /* Find next prime greater than last_prime */
        uint64_t candidate = last_prime + 2; /* skip even numbers */
        while (!is_prime_big((seq_t)candidate)) {
            candidate += 2;
        }

        /* Count representations of candidate as arithmetic mean of two primes */
        size_t cnt = 0;
        seq_t two_candidate = (seq_t)candidate * 2;

        for (size_t i = 0; i < prime_cnt; ++i) {
            seq_t q = two_candidate - (seq_t)prime_list[i];
            if (q >= 2 && is_prime_big(q)) {
                ++cnt;
                /* We only care about counts up to the next term index */
                if (cnt > terms_len + 1) {
                    /* No need to continue; candidate cannot be the next term */
                    break;
                }
            }
        }

        /* If the count equals the next required index, store the term */
        if (cnt == terms_len + 1) {
            ensure_capacity(terms_len + 1, &terms_cap, OEISC_VPP(&terms), sizeof(seq_t));
            terms[terms_len++] = (seq_t)candidate;
        }

        /* Add the candidate to the list of primes for future counts */
        ensure_capacity(prime_cnt + 1, &prime_cap, OEISC_VPP(&prime_list), sizeof(uint64_t));
        prime_list[prime_cnt++] = candidate;
        last_prime = candidate;
    }

    /* Return the nth term (1‑based indexing) */
    return terms[n - 1];
}
