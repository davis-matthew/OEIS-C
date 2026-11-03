/*
 * A175449  a(n)>a(n-1), a(n) = smallest prime such that a(n)+a(n-1) is
 *          multiple of m, a(1)=2, m=17.
 * https://oeis.org/A175449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; for n>1, a(n) is the smallest prime greater than a(n-1)
 *            such that a(n)+a(n-1) is a multiple of 17. The function memoizes
 *            the sequence, so calls in any order remain efficient
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

/* Memoization array for the sequence values. */
static seq_t *cache = NULL;   /* cache[i] holds a(i+1) */
static size_t cache_cap = 0;  /* current allocated size */

/* Return the n-th element of the sequence (a(1)=2). */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined index – return 0 */
        return (seq_t)0;

    /* initialise cache with a(1)=2 */
    if (cache == NULL) {
        cache = mem_new(seq_t, 1);
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)2;
        cache_cap = 1;
    }

    /* Grow cache if needed, doubling size for efficiency */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;  /* double */
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (tmp == NULL) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms up to n if not already cached */
    if (cache[0] == 0) {                 /* safety check */
        cache[0] = (seq_t)2;
    }
    for (idx_t i = 1; i < n; ++i) {      /* i is zero‑based index for a(i+1) */
        seq_t prev = cache[i-1];
        /* desired residue r such that (cand + prev) % 17 == 0 */
        const seq_t MOD17 = (seq_t)17;
        seq_t rem_prev = prev % MOD17;
        seq_t r = (MOD17 - rem_prev) % MOD17;

        /* start searching with the first integer > prev */
        seq_t cand = prev + (seq_t)1;
        /* skip to the next odd number (after the start) */
        if (cand % (seq_t)2 == (seq_t)0 && cand != (seq_t)2)
            cand += (seq_t)1;

        /* search until a prime with the required residue is found */
        while (true) {
            if ((cand % MOD17) == r && is_prime_big(cand))
                break;
            cand += (seq_t)1;
            /* jump over evens except for number 2 */
            if (cand % (seq_t)2 == (seq_t)0 && cand != (seq_t)2)
                cand += (seq_t)1;
        }
        cache[i] = cand;
    }

    return cache[n-1];
}
