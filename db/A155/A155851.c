/*
 * A155851  n is prime and is the sum of the first k primes for some k, start
 *          from 5.
 * https://oeis.org/A155851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime that equals the sum of the first k primes
 *            starting from the prime 5 (i.e., sums of consecutive primes
 *            5,7,11,13,⋯)
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
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
    static bool initialized = false;
    static seq_t *cache  = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t cumulative_sum = 0;
    static uint64_t current_prime = 5;   /* prime to add next */
    static uint64_t next_candidate = 7;  /* candidate for next prime generation */

    if (n < 1)
        return (seq_t)0;

    if (!initialized) {
        cache  = NULL;
        cache_len = 0;
        cache_cap = 0;
        cumulative_sum = 0;
        current_prime = 5;
        next_candidate = 7;
        initialized = true;
    }

    /* Generate terms until we have at least n entries cached. */
    while (cache_len < n) {
        /* Add the current prime to the running sum. */
        cumulative_sum = cumulative_sum + (seq_t)current_prime;

        /* If the resulting sum is prime, store it. */
        if (is_prime_big(cumulative_sum))
            append_candidate(&cache, &cache_len, &cache_cap, cumulative_sum);

        /* Find the next prime after current_prime. */
        uint64_t cand = current_prime + 2;   /* start from next odd */
        while (!is_prime((idx_t)cand))
            cand += 2;
        current_prime = cand;
    }

    return cache[n - 1];
}
