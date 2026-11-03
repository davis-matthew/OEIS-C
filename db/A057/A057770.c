/*
 * A057770  The primes in A045574.
 * https://oeis.org/A057770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) is the nth prime whose decimal digits belong to the set
 *            {0,1,6,8,9}
 * Range:     u8 n <= 9, u16 n <= 178, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 968 (2026-11-03)
 * Helpers:   digit_frequencies, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_frequencies.h"
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

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* candidate for testing, start from the smallest possible prime 2 */
    static seq_t candidate = 2;

    /* return cached element if already present */
    if (cache_size >= n)
        return cache[n - 1];

    /* generate primes until we have n entries */
    while (cache_size < n) {
        /* check digit set using utility */
        unsigned int freq[10] = {0};
        digit_frequencies(candidate, freq);
        if (freq[2] == 0 && freq[3] == 0 &&
            freq[4] == 0 && freq[5] == 0 &&
            freq[7] == 0) {
            /* candidate has only allowed digits */
            if (is_prime_big(candidate)) {
                /* enlarge cache if necessary */
                if (cache_size == cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    grow_seq(&cache, &cache_cap, new_cap);
                }
                cache[cache_size++] = candidate;
            }
        }
        /* advance to next integer */
        candidate = candidate + 1;
    }

    return cache[n - 1];
}
