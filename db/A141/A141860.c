/*
 * A141860  Primes congruent to 2 mod 15.
 * https://oeis.org/A141860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 820, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 909 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th prime p such that p % 15 == 2   (offset 1) */
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

seq_t a(idx_t n)
{
    /* Static cache of found primes congruent to 2 mod 15. */
    static seq_t *cache   = NULL;     /* array of values   */
    static size_t cap     = 0;        /* allocated capacity */
    static size_t len     = 0;        /* number of stored primes */
    static seq_t last     = 0;        /* last prime stored */

    if (n == 0)                    /* undefined for offset 1 */
        return 0;

    /* Allocate initial cache if necessary. */
    if (cache == NULL) {
        cap = 16;
        cache = mem_new(seq_t, cap);
        if (cache == NULL)         /* allocation failure */
            return 0;
    }

    /* Generate primes until we have at least n entries. */
    while (len < n) {
        seq_t candidate = (len == 0) ? (seq_t)2 : last + (seq_t)15;

        /* Find the next prime congruent to 2 mod 15. */
        while (!is_prime_big(candidate))
            candidate += (seq_t)15;

        /* Grow cache if needed. */
        if (len == cap) {
            cap <<= 1;                      /* double the capacity */
            seq_t *new_cache = mem_resize(seq_t, cache, cap);
            if (new_cache == NULL)          /* allocation failure */
                return 0;
            cache = new_cache;
        }

        cache[len++] = candidate;
        last = candidate;
    }

    return cache[n - 1];
}
