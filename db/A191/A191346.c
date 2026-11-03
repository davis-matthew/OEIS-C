/*
 * A191346  Primes = 1 mod 17#.
 * https://oeis.org/A191346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime congruent to 1 modulo 17# (510510)
 * Range:     u8 none, u16 none, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-03)
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

/* Returns the n-th term of OEIS A191346.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of found primes */
    static size_t cap   = 0;      /* current capacity of cache   */
    static size_t sz    = 0;      /* number of primes stored     */
    static idx_t  next_k = 1;     /* k for the next candidate (510510*k + 1) */

    /* Magic modulus: 17# = 2·3·5·7·11·13·17 = 510510 */
    static const seq_t MOD  = 510510;
    static const seq_t OFF  = 1;

    /* Return cached value if already computed. */
    if (n <= sz)
        return cache[n - 1];

    /* Generate new candidates until we have at least n primes. */
    while (sz < (size_t)n) {
        seq_t cand = MOD * (seq_t)next_k + OFF;  /* candidate = 510510*k + 1 */
        ++next_k;

        if (is_prime_big(cand)) {
            /* Grow the cache array if needed. */
            if (sz == cap) {
                size_t new_cap = cap ? cap * 2 : 8;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return 0;           /* allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[sz++] = cand;              /* store the found prime */
        }
    }

    return cache[n - 1];
}
