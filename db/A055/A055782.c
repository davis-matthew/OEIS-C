/*
 * A055782  Primes q of the form q = 10p + 3, where p is also prime.
 * https://oeis.org/A055782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 318, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9923 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = 10 * p + 3 where p is prime and 10p + 3 is also prime. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include "../utils/is_prime.h"
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

/* Return true if the 64‑bit value fits into the target seq_t type. */
static inline bool fits_seq_t(uint64_t val)
{
    if (sizeof(seq_t) == 1) return val <= 0xFFULL;
    if (sizeof(seq_t) == 2) return val <= 0xFFFFULL;
    if (sizeof(seq_t) == 4) return val <= 0xFFFFFFFFULL;
    if (sizeof(seq_t) == 8) return val <= 0xFFFFFFFFFFFFFFFFULL;
    /* Larger or arbitrary‑precision types are assumed to handle any value. */
    return true;
}

/* Return the n-th term of A055782 (offset 1). */
seq_t a(idx_t n)
{
    /* Cache of computed terms. */
    static seq_t *cache      = NULL;   /* sequence values */
    static size_t cache_size = 0;      /* number of values stored */
    static size_t cache_cap  = 0;      /* current allocation size */
    static idx_t  last_p     = 1;      /* last examined prime p */

    if (n == 0) return (seq_t)0;  /* undefined behaviour for offset 1 */

    /* Generate terms until the requested index is available. */
    while (cache_size < (size_t)n)
    {
        /* Find the next prime p. */
        do
        {
            ++last_p;
        } while (!is_prime(last_p));

        /* Compute q = 10 * p + 3, using a 64‑bit intermediate to avoid overflow. */
        uint64_t qtemp = 10ULL * (uint64_t)last_p + 3ULL;

        /* Store q only if it fits into seq_t and is prime. */
        if (fits_seq_t(qtemp) && is_prime_big((seq_t)qtemp))
        {
            grow_seq(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = (seq_t)qtemp;
        }
    }

    return cache[n - 1];
}
