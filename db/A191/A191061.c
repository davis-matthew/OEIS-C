/*
 * A191061  Primes p that have Kronecker symbol (p|14) = -1.
 * https://oeis.org/A191061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 990 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine returns the n‑th prime p with Kronecker symbol (p|14) = -1.
   Equivalently, p ≡ 11,17,29,31,33,37,41,43,47,51,53,55 (mod 56). */

/* Embedded search of successive primes in the specified residue classes,
   with a simple dynamic cache so that calls with arbitrary n are supported. */

#include <stdbool.h>
#include <stddef.h>
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

/* Check whether a prime is in one of the six residue classes modulo 56. */
static bool in_residue_set(seq_t p)
{
    /* The result of % with a small constant is guaranteed to fit in an int. */
    int r = (int)(p % 56u);
    switch (r) {
        case 11: case 17: case 29: case 31:
        case 33: case 37: case 41: case 43:
        case 47: case 51: case 53: case 55:
            return true;
        default:
            return false;
    }
}

/* Return the n‑th term (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Static cache of found primes. */
    static seq_t *cache = NULL;   /* array of primes */
    static idx_t count = 0;       /* number of cached primes */
    static idx_t cap = 0;         /* allocated capacity */
    static seq_t candidate = 3;   /* next odd number to test */

    /* Return cached result if already available. */
    if (n <= count)
        return cache[n - 1];

    /* Generate successive primes until we have n terms. */
    while (count < n) {
        if (is_prime_big(candidate)) {
            if (in_residue_set(candidate)) {
                /* Grow the cache if needed. */
                if (count == cap) {
                    idx_t new_cap = cap == 0 ? 16 : cap * 2;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp)  /* allocation failed – abort gracefully */
                        return 0;
                    cache = tmp;
                    cap   = new_cap;
                }
                cache[count++] = candidate;
            }
        }
        candidate += 2;   /* skip even numbers */
    }

    return cache[n - 1];
}
