/*
 * A049985  Primes of form k! - (k-1)! - 1.
 * https://oeis.org/A049985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime of the form k! - (k-1)! - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t *cache = NULL;          /* array of computed primes */
static size_t cache_cap = 0;         /* capacity of the cache */
static idx_t cache_len = 0;          /* number of primes stored */

static idx_t last_k = 1;             /* k for which fact_cur = k! */
static seq_t fact_cur = (seq_t)1;    /* k!  (value of factorial for last_k) */
static seq_t fact_prev = (seq_t)1;   /* (k-1)! for the next iteration */

static void compute_upto(idx_t n)
{
    while (cache_len < n)
    {
        /* advance to next k */
        ++last_k;

        /* prev factorial is current factorial before multiplication */
        fact_prev = fact_cur;

        /* fact_cur = fact_prev * last_k = last_k! */
        fact_cur *= (seq_t)last_k;

        /* term = k! - (k-1)! - 1 */
        seq_t term = fact_cur - fact_prev - (seq_t)1;

        /* if term is prime, store it */
        if (is_prime_big(term))
        {
            if (cache_len == cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = term;
        }
    }
}

seq_t a(idx_t n)
{
    if (cache == NULL)
    {
        cache_cap = 8;
        cache = seq_alloc(cache_cap);
    }

    if (n == 0)
        return (seq_t)0;   /* undefined for n<1, but return 0 */

    compute_upto(n);
    return cache[n - 1];
}
