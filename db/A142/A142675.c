/*
 * A142675  Primes congruent to 16 mod 57.
 * https://oeis.org/A142675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime p such that p ≡ 16 (mod 57)
 * Range:     u8 n <= 1, u16 n <= 178, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 976 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates candidates 57k+16 and tests them for primality.
   It keeps a static cache of discovered primes so subsequent calls are fast. */

#include <stddef.h>
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
    static seq_t *cache     = NULL;   /* stored primes  */
    static size_t cap       = 0;      /* capacity          */
    static size_t sz        = 0;      /* number of primes  */
    static seq_t last_cand  = 0;      /* last tested candidate */

    /* initialise cache with the first known prime 73 */
    if (sz == 0) {
        cap = 64;
        cache = mem_new(seq_t, cap);
        if (!cache) return 0;               /* unlikely failure */
        cache[0] = (seq_t)73;               /* first prime ≡ 16 (mod 57) */
        sz = 1;
        last_cand = (seq_t)73;
    }

    /* generate further primes until reaching the requested index */
    while (sz < n) {
        last_cand += (seq_t)57;                 /* next number ≡ 16 (mod 57) */
        if (is_prime_big(last_cand)) {           /* primality test */
            if (sz == cap) {                    /* grow cache if needed */
                cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, cache, cap);
                if (!tmp) return 0;              /* allocation failure */
                cache = tmp;
            }
            cache[sz++] = last_cand;
        }
    }

    return cache[n - 1];
}
