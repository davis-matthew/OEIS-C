/*
 * A142454  Primes congruent to 47 mod 49.
 * https://oeis.org/A142454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 142, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 998 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142454: primes congruent to 47 mod 49 */
/* Computation: iterate candidates p = 47 + 49*k, test primality by is_prime_big */

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

static seq_t *cache = NULL;        /* array of found primes */
static size_t cap = 0;             /* allocated capacity */
static idx_t  max_n = 0;           /* number of primes stored */
static seq_t  next_candidate = 47; /* next number to test, congruent to 47 mod 49 */

/* grow cache to accommodate at least need primes */
static void ensure_cache(idx_t n)
{
    const seq_t inc = (seq_t)49;
    while (max_n < n)
    {
        if (is_prime_big(next_candidate))
        {
            /* extend cache if necessary */
            if (max_n >= cap)
            {
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, newcap);
                if (!tmp) abort();             /* out‑of‑memory */
                cache = tmp;
                cap   = newcap;
            }
            cache[max_n] = next_candidate;
            ++max_n;
        }
        next_candidate += inc;
    }
}

/* external function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n >= 1 */
    ensure_cache(n);
    return cache[n - 1];
}
