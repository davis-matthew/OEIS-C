/*
 * A175446  a(n)>a(n-1), a(n) = smallest prime such that a(n)+a(n-1) is
 *          multiple of m, a(1)=2, m=11.
 * https://oeis.org/A175446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; for n>1, a(n) = the smallest prime > a(n-1) * such
 *            that (a(n) + a(n-1)) mod 11 == 0. * a(1)=2, a(2)=31, a(3)=79,
 *            a(4)=97, …
 * Range:     u8 n <= 10, u16 n <= 800, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 889 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
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

/* returns the nth term of the sequence (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1, so n==0 is undefined */
        return 0;

    /* Cache for previously computed terms.  cache[i] holds a(i+1). */
    static seq_t *cache     = NULL;
    static idx_t cache_size = 0;  /* number of stored terms */
    static size_t cache_cap = 0;  /* allocated capacity of cache */

    /* Initialise the cache with a(1) if it is empty. */
    if (cache_size == 0) {
        grow_seq(&cache, &cache_cap, 1);
        cache[0] = (seq_t)2;   /* a(1) */
        cache_size = 1;
    }

    /* Grow the cache up to n if necessary. */
    if (n > cache_size) {
        grow_seq(&cache, &cache_cap, (size_t)n);

        for (idx_t i = cache_size; i < n; ++i) {        /* compute a(i+1) */
            seq_t prev = cache[i - 1];                 /* a(i)      */
            /* Residue needed so that (prev + candidate) % 11 == 0 */
            seq_t target_mod = (11 - (prev % 11)) % 11;

            /* First candidate > prev with this residue. */
            seq_t cand = prev + (seq_t)1;
            seq_t cand_mod = cand % 11;
            seq_t delta;
            if (cand_mod <= target_mod)
                delta = target_mod - cand_mod;
            else
                delta = target_mod + 11 - cand_mod;
            cand += delta;
            if (cand <= prev)                           /* safety */
                cand += 11;

            /* Search for the first prime with the required property. */
            while (!is_prime_big(cand))
                cand += 11;

            cache[i] = cand;
        }

        cache_size = n;
    }

    return cache[n - 1];
}
