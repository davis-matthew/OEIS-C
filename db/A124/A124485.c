/*
 * A124485  Numbers k such that both 2*k-1 and 4*k-1 are primes.
 * https://oeis.org/A124485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that 2k–1 and 4k–1 are primes
 * Range:     u8 n <= 11, u16 n <= 885, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 72 terms through n = 4663 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *k_cache = NULL;      /* cache of found terms */
static size_t k_len  = 0;          /* number of cached terms   */
static size_t k_cap  = 0;          /* current capacity         */
static idx_t  last_k_checked = 1;  /* last k processed         */

/* enlarge the cache when needed */
static void ensure_k_capacity(size_t needed)
{
    if (k_cap < needed) {
        size_t newcap = k_cap ? k_cap << 1 : 1;
        while (newcap < needed) newcap <<= 1;
        k_cache = seq_realloc(k_cache, k_cap, newcap);
        k_cap   = newcap;
    }
}

/* find the next k > last_k_checked that satisfies the condition */
static void find_next_k(void)
{
    for (idx_t k = last_k_checked + 1; ; ++k) {
        seq_t p1 = ((seq_t)k * ((seq_t)2)) - ((seq_t)1);
        seq_t p2 = ((seq_t)k * ((seq_t)4)) - ((seq_t)1);
        if (is_prime_big(p1) && is_prime_big(p2)) {
            seq_t kval = (seq_t)k;
            ensure_k_capacity(k_len + 1);
            k_cache[k_len++] = kval;
            last_k_checked   = k;
            break;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */
    while (k_len < (size_t)n)                  /* extend cache if needed */
        find_next_k();
    return k_cache[(size_t)n - 1];              /* return the nth term */
}
