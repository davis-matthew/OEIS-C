/*
 * A159082  Numbers whose squares added to 7! are prime.
 * https://oeis.org/A159082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth positive integer a such that 7! + a^2 is prime
 * Range:     u8 none, u16 n <= 19, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 938 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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
    /* Static cache of discovered sequence values. */
    static seq_t *cache      = NULL;   /* array of a(k) */
    static size_t cache_cap = 0;       /* current allocation size */
    static size_t cache_sz  = 0;       /* number of values currently stored */
    static uint64_t next_candidate = 1;/* last candidate examined (starts at 1) */

    /* Extend cache if necessary. */
    if (cache_sz + 1 > cache_cap)
        grow_seq(&cache, &cache_cap, cache_sz + 1);

    /* Generate additional terms until reaching index n. */
    while (cache_sz < n) {
        ++next_candidate; /* consider the next integer candidate */

        /* Skip obvious composites: a must not be divisible by 2,3,5,7. */
        if ((next_candidate & 1ULL) == 0ULL ||
            (next_candidate % 3ULL) == 0ULL ||
            (next_candidate % 5ULL) == 0ULL ||
            (next_candidate % 7ULL) == 0ULL)
            continue;

        /* Compute 7! + a^2 using the generic seq_t type. */
        seq_t cand = (seq_t)next_candidate;
        seq_t p = (seq_t)5040 + cand * cand;

        /* Check primality of the resulting number. */
        if (is_prime_big(p)) {
            /* Store the new term in the cache. */
            if (cache_sz + 1 > cache_cap)
                grow_seq(&cache, &cache_cap, cache_sz + 1);
            cache[cache_sz++] = cand;
        }
    }

    return cache[n - 1];
}
