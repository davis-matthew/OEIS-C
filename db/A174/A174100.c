/*
 * A174100  Numbers k such that both 2*k + 1 and 6*k + 1 are prime.
 * https://oeis.org/A174100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k (k ≥ 1) such that 2*k+1 and 6*k+1 are
 *            prime
 * Range:     u8 n <= 13, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

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

seq_t a(idx_t n)
{
    /* a(1) is the first term.  The function is defined for n ≥ 1.  */
    if (n == 0) return 0;  /* defensive, not used for valid offset */

    /* Static memoised cache of found k’s.  */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* how many terms are stored */
    static size_t cache_cap  = 0;   /* capacity of the cache array */
    static idx_t  last_k     = 0;   /* highest k that has been examined */

    /* Allocate or grow the cache array when needed.  */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* Generate new terms until the cache contains at least n entries.  */
    if (cache_len < (size_t)n) {
        for (idx_t k = last_k + 1; cache_len < (size_t)n; ++k) {
            last_k = k;
            seq_t sk = (seq_t)k;
            seq_t t1 = sk * 2 + 1;
            seq_t t2 = sk * 6 + 1;
            if (is_prime_big(t1) && is_prime_big(t2)) {
                cache[cache_len++] = sk;
            }
        }
    }

    /* Return the nth term (1‑based).  */
    return cache[n - 1];
}
