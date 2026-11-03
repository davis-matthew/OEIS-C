/*
 * A141962  Primes congruent to 22 mod 27.
 * https://oeis.org/A141962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime congruent to 22 modulo 27
 * Range:     u8 n <= 3, u16 n <= 368, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 892 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    /* Static cache of primes with residue 22 mod 27 */
    static seq_t *cache = NULL;          /* Dynamically allocated array */
    static size_t cache_size  = 0;       /* Number of elements stored */
    static size_t cache_cap   = 0;       /* Current capacity of the array */

    /* The last value of k such that candidate = 27*k + 22 was examined.
       It starts at 1, so the first candidate considered is 49. */
    static idx_t last_k = 1;

    /* Expand the cache until we have at least n elements. */
    while (cache_size < n)
    {
        /* Candidate number of the form 27*k + 22 */
        seq_t candidate = (seq_t)27 * (seq_t)last_k + 22;
        last_k++;  /* Next candidate will use next k */

        /* If candidate is prime, store it. */
        if (is_prime_big(candidate))
        {
            /* Ensure the cache has enough capacity */
            if (cache_cap <= cache_size)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : (cache_cap * 2);
                void *tmp = mem_resize(seq_t, cache, new_cap);
                cache = (seq_t *)tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = candidate;
        }
    }

    /* Return the (n-1)-th element (offset 1). */
    return cache[n - 1];
}
