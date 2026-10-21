/*
 * A097617  8-digit primes formed by concatenating the first decimal digits of
 *          {2^n, ..., 9^n} with the n's given by A097616.
 * https://oeis.org/A097617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth 8‑digit prime obtained by concatenating the *
 *            leading decimal digit of 2^k, 3^k, … , 9^k for successive k,
 *            skipping * non‑prime concatenations. In particular
 *            a(1)=23456789, a(2)=21443183, …
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_uint32, modpow_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/is_prime_uint32.h"
#include "../utils/modpow_uint32.h"

/* first decimal digit of i^k (2 ≤ i ≤ 9) using high‑precision arithmetic */
static unsigned leading_digit(unsigned i, idx_t k)
{
    long double exponent = (long double)k * log10l((long double)i);
    long double frac = exponent - floorl(exponent);
    long double t = powl(10.0L, frac);
    unsigned d = (unsigned)(t + 1e-12L);   /* tiny epsilon to avoid 9.999… */
    if (d == 10U) d = 9U;
    return d;
}

/* sequence a(n) as defined above */
seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)0;   /* offset is 1; value below is undefined */

    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cached = 0;
    static size_t cap = 0;

    /* next exponent to test (k) */
    static idx_t next_k = 1U;

    while (cached < n)
    {
        uint32_t val = 0U;
        for (unsigned base = 2U; base <= 9U; ++base)
        {
            unsigned d = leading_digit(base, next_k);
            val = val * 10U + d;
        }

        if (is_prime_uint32(val))
        {
            if (cached + 1U > cap)
            {
                size_t new_cap = cap ? cap : 16U;
                while (new_cap <= cached + 1U) new_cap <<= 1U;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[cached] = (seq_t)val;
            ++cached;
        }
        ++next_k;
    }

    return cache[n - 1U];
}
