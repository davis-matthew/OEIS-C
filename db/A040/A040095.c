/*
 * A040095  Primes p such that x^3 = 23 has a solution mod p.
 * https://oeis.org/A040095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 906 (2026-11-03)
 * Helpers:   is_prime, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A040095: primes p such that x^3 = 23 has a solution mod p
 *  Algorithm: generate primes in order and keep those for which 23 is a cubic
 *  residue.  For a prime p, 23 is a cubic residue iff
 *      p = 3          (trivial)
 *   OR  p % 3 == 2    (all non‑trivial residues in this class)
 *   OR  p % 3 == 1  AND 23^((p-1)/3) ≡ 1 (mod p)
 *  The function memoises all found primes so that any call a(n) is fast.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <string.h>
#include "../utils/is_prime.h"
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

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;   /* array of found primes */
    static size_t cache_len = 0;  /* number of elements stored */
    static size_t cache_cap = 0;  /* current capacity of 'cache' */
    static uint64_t last_checked = 2;  /* next candidate to test */

    /* generate primes until we have at least n of them */
    while (cache_len < n) {
        uint64_t p = last_checked;

        /* update last_checked for next loop */
        last_checked = p + 1;
        /* skip even numbers larger than 2 */
        if (p > 2 && (p & 1) == 0)
            continue;

        /* primality test */
        if (!is_prime(p))
            continue;

        /* test whether 23 is a cubic residue modulo p */
        bool ok;
        if (p == 3) {
            ok = true;
        } else if ((p % 3) == 2) {
            ok = true;
        } else { /* p % 3 == 1 */
            seq_t r = powmod((seq_t)23, (p - 1) / 3, (seq_t)p);
            ok = (r == (seq_t)1);
        }

        if (ok)
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)p);
    }

    return cache[n - 1];
}
