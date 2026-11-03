/*
 * A142072  Primes congruent to 19 mod 34.
 * https://oeis.org/A142072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 407, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 980 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Compute the n-th prime that is congruent to 19 modulo 34.
 * Uses a simple incremental search and memoisation.
 * The function is thread‑safe only if called from a single thread.
 * Suitable for int, 64‑bit, or arbitrary‑precision seq_t.
 */
#include <stdbool.h>
#include <stddef.h>
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

seq_t a(idx_t n)
{
    /* trivial case: below offset (n < 1) */
    if (n < 1)
        return 0;

    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t sz = 0;      /* number of stored terms */
    static size_t cap = 0;     /* allocated capacity */

    /* candidate value of the next check (always 19 mod 34) */
    static seq_t candidate = 19;
    static bool initialized = false;

    /* initialise candidate on first call */
    if (!initialized)
    {
        candidate = 19;
        initialized = true;
    }

    /* return from cache if already computed */
    if (n <= sz)
        return cache[n - 1];

    /* expand cache until we have at least n terms */
    while (sz < n)
    {
        /* find next prime of form 19 mod 34 */
        while (!is_prime_big(candidate))
            candidate += 34;

        /* grow cache if necessary */
        if (sz == cap)
            grow_seq(&cache, &cap, sz + 1);

        /* store prime */
        cache[sz++] = candidate;

        /* move to next candidate (19 mod 34) */
        candidate += 34;
    }

    return cache[n - 1];
}
