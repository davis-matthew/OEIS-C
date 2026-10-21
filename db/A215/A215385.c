/*
 * A215385  Primes congruent to {1, 2, 3, 6} mod 19.
 * https://oeis.org/A215385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) = the nth prime p with p mod 19 ∈ {1,2,3,6} */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Grow a dynamic array of seq_t */
static void ensure_cap(seq_t **arr, size_t *cap, size_t need)
{
    if (need > *cap) {
        size_t new_cap = *cap ? *cap * 2 : 128;
        while (new_cap < need) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Integer square root for idx_t (64‑bit unsigned).  Avoids overflow. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid > 0 && mid > x / mid)  /* mid*mid > x  */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Global storage for primes and for the wanted special primes */
static seq_t *primes   = NULL;
static size_t  prime_cap   = 0, prime_cnt = 0;
static seq_t *specials  = NULL;
static size_t special_cap = 0, special_cnt = 0;
static seq_t candidate = 3;
static bool initialized = false;

/* Return the nth prime whose residue modulo 19 is 1, 2, 3 or 6. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, but return 0 */

    /* Initialise the small tables once */
    if (!initialized) {
        ensure_cap(&primes, &prime_cap, 1);
        primes[0] = 2;
        prime_cnt = 1;

        ensure_cap(&specials, &special_cap, 1);
        specials[0] = 2;                  /* 2 % 19 == 2 */
        special_cnt = 1;

        candidate = 3;
        initialized = true;
    }

    /* Keep extending until we have at least n special primes */
    while ((size_t)n > special_cnt) {
        seq_t p = candidate;

        /* Primality test by trial division using previously found primes */
        bool is_prime = true;
        idx_t lim = isqrt_u64((idx_t)p);
        for (size_t i = 0; i < prime_cnt && primes[i] <= lim; ++i) {
            if (p % primes[i] == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* Store the new prime */
            ensure_cap(&primes, &prime_cap, prime_cnt + 1);
            primes[prime_cnt++] = p;

            /* Check the residue condition */
            seq_t r = p % 19;
            if (r == 1 || r == 2 || r == 3 || r == 6) {
                ensure_cap(&specials, &special_cap, special_cnt + 1);
                specials[special_cnt++] = p;
            }
        }

        candidate += 2;   /* skip even numbers */
    }

    return specials[n - 1];
}
