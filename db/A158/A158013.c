/*
 * A158013  100*a(n)+13 and 100*a(n)+27 are consecutive primes, i.e., a prime
 *          gap 14.
 * https://oeis.org/A158013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A158013: a(n) is the n‑th integer a such that
 * 100*a+13 and 100*a+27 are consecutive primes (prime gap 14). */
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

static seq_t *cache      = NULL;
static idx_t cache_size  = 0;
static idx_t cap         = 0;
static idx_t last_checked = 0;        /* last a that was examined */

seq_t a(idx_t n)
{
    /* Return cached value if available. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Initialise or grow the cache. */
    if (cap == 0) {
        cap   = 32;
        cache = mem_new(seq_t, cap);
        if (!cache) exit(EXIT_FAILURE);
    }

    /* Search for new terms until we obtain the nth. */
    for (;;) {
        /* Candidate a. */
        last_checked++;
        seq_t a_candidate = last_checked;

        /* Endpoints of the 14‑digit range. */
        seq_t p1 = a_candidate * (seq_t)100 + (seq_t)13;
        seq_t p2 = p1 + (seq_t)14;          /* 100*a+27 */

        /* Both must be prime. */
        if (!is_prime_big(p1) || !is_prime_big(p2))
            continue;

        /* Check for primes strictly between p1 and p2.
         * The only odd offsets in (13,27) are 4,6,8,10,12. */
        if (is_prime_big(p1 + (seq_t)4) ||
            is_prime_big(p1 + (seq_t)6) ||
            is_prime_big(p1 + (seq_t)8) ||
            is_prime_big(p1 + (seq_t)10)||
            is_prime_big(p1 + (seq_t)12))
            continue;

        /* Valid a – store it. */
        if (cache_size == cap) {
            cap *= 2;
            seq_t *tmp = mem_resize(seq_t, cache, cap);
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
        }
        cache[cache_size++] = a_candidate;

        /* If we have reached the requested index, return it. */
        if (cache_size == n)
            return cache[n - 1];
    }
}
