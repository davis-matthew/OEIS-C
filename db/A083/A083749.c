/*
 * A083749  Brilliant star numbers: semiprimes of the form 6n^2 - 6n + 1 such
 *          that both prime factors have the same number of decimal digits.
 * https://oeis.org/A083749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A083749 – Brilliant star numbers
 *
 * Definition: a(n) is the nth number of the form 6k² – 6k + 1 that
 * is a semiprime, and whose two prime factors have the same
 * number of decimal digits.  The index n counts how many such
 * numbers have been found.  For example a(1)=121, a(2)=253, …
 *
 * The function a() generates terms on demand.  It keeps an internal
 * cache of all discovered terms and continues the search from the
 * last examined k until enough terms are obtained.
 *
 * Complexity:  O(number of examined k) – each candidate is
 * factored with a fast trial‑division routine from the utilities.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* --------------------------------------------------------------- */
static void factor(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p*p <= n; ++p) {
        if (n % p == 0) {
            pr[c] = p; exp[c] = 0;
            while (n % p == 0) { n /= p; ++exp[c]; }
            ++c;
        }
    }
    if (n > 1) { pr[c] = n; exp[c] = 1; ++c; }
    *cnt = c;
}

/* number of decimal digits of a positive 64‑bit integer */
static uint8_t dec_digits_uint64(uint64_t x)
{
    uint8_t d = 0;
    do { ++d; x /= 10; } while (x);
    return d;
}

/* sequence term function */
seq_t a(idx_t n)
{
    /* static state for caching */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t  next_k   = 1;          /* next k to test */

    /* grow cache until we have at least n terms */
    while (cache_len < n)
    {
        /* compute value 6k^2 - 6k + 1 using 128‑bit arithmetic */
        unsigned __int128 t = (unsigned __int128)6 * (unsigned __int128)next_k *
                             (unsigned __int128)next_k
                            - (unsigned __int128)6 * (unsigned __int128)next_k
                            + 1;
        uint64_t val = (uint64_t)t;   /* fits 64 bits for n up to 10 000 */

        /* factor val into primes */
        uint64_t primes[4];
        uint64_t exps  [4];
        size_t   cnt   = 0;
        factor(val, primes, exps, &cnt);

        bool term_ok = false;

        if (cnt == 2 && exps[0] == 1 && exps[1] == 1)          /* two distinct primes */
        {
            if (dec_digits_uint64(primes[0]) == dec_digits_uint64(primes[1]))
                term_ok = true;
        }
        else if (cnt == 1 && exps[0] == 2)                      /* square of a prime */
        {
            term_ok = true;
        }

        if (term_ok)
        {
            /* extend cache if necessary */
            if (cache_len == cache_cap)
            {
                cache_cap = (cache_cap ? cache_cap * 2U : 16U);
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = (seq_t)val;
        }

        ++next_k;
    }

    return cache[n - 1];
}
