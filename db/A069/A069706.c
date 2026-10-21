/*
 * A069706  Primes with property that swapping first and last digits also
 *          gives a prime.
 * https://oeis.org/A069706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1350, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   swap_first_last
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search incrementally for primes whose first and last decimal
   digits swapped yield another prime; Miller–Rabin is used for primality. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/swap_first_last.h"

/* Deterministic Miller‑Rabin for 64‑bit integers.
   Witnesses {2, 325, 9375, 28178, 450775, 9780504, 1795265022}
   are sufficient for all n < 2^64. */
static uint64_t mulmod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t powmod64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t res = 1;
    a %= mod;
    while (d) {
        if (d & 1)
            res = mulmod64(res, a, mod);
        a = mulmod64(a, a, mod);
        d >>= 1;
    }
    return res;
}

static int is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return 0;
    static const uint64_t small_primes[] = {2, 3, 5, 7, 11, 13, 17,
                                            19, 23, 29, 31, 37};
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0)
            return n == p;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t witnesses[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof(witnesses) / sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0)
            continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        int j;
        for (j = 1; j < (int)s; ++j) {
            x = mulmod64(x, x, n);
            if (x == n - 1)
                break;
        }
        if (j == (int)s)
            return 0;
    }
    return 1;
}

/* Dynamic array of found special primes (type seq_t).  */
static seq_t *special_list = NULL;
static size_t   special_cap = 0;
static idx_t    special_len = 0;

static void push_special(seq_t v)
{
    if ((size_t)special_len == special_cap) {
        size_t new_cap = special_cap ? special_cap << 1 : 8;
        seq_t *tmp = (seq_t *)realloc(special_list,
                                      new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        special_list = tmp;
        special_cap  = new_cap;
    }
    special_list[special_len++] = v;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for 0, harmless */

    static uint64_t current = 2;   /* next integer to test */
    while (special_len < n) {
        while (!is_prime_uint64(current))
            ++current;          /* find next prime */

        /* current is prime */
        uint64_t swapped = swap_first_last(current);
        if (is_prime_uint64(swapped))
            push_special((seq_t)current);

        ++current;              /* move ahead for next search */
    }
    return special_list[n - 1];
}
