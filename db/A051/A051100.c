/*
 * A051100  Primes p such that x^62 = -2 has a solution mod p.
 * https://oeis.org/A051100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime p such that the congruence x^62 = -2 (mod
 *            p) has a solution
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 99 terms through n = 908 (2026-11-03)
 * Helpers:   gcd_u64, mem, next_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/next_prime.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Euclidean algorithm for uint64_t */

seq_t a(idx_t n)
{
    /* Static cache for the sequence terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_prime = 0;   /* 0 means no prime processed yet */

    /* Grow cache until we have n terms */
    while (cache_len < (size_t)n) {
        uint64_t p;
        if (last_prime == 0)
            p = next_prime(0);      /* first prime = 2 */
        else
            p = next_prime(last_prime);  /* next prime after last_prime */
        last_prime = p;

        bool ok;
        if (p == 2) {
            /* x^62 = -2 mod 2 is always solvable (x=0 gives 0 = 0) */
            ok = true;
        } else {
            uint64_t g   = gcd_u64(62ULL, p - 1);
            uint64_t exp = (p - 1) / g;         /* exponent for the power test */
            seq_t  base = (seq_t)(p - 2);      /* -2 modulo p */
            seq_t  res  = powmod(base, exp, (seq_t)p);
            ok = (res == (seq_t)1);
        }

        if (ok) {
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)p);
        }
    }

    return cache[(size_t)n - 1];
}
