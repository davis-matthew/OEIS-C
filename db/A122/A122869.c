/*
 * A122869  Primes p that divide Lucas((p-1)/2), where Lucas is A000032.
 * https://oeis.org/A122869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 982, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p such that Lucas((p-1)/2) mod p == 0 */
/* This implementation generates primes on demand, checks each for the
   property using fast doubling for Fibonacci (and derived Lucas
   numbers), and caches found primes for future calls. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/next_prime.h"
#include "../utils/alloc_seq_array.h"

/* Multiply two seq_t numbers modulo mod.
   For 8/16/32/64-bit seq_t we use 128‑bit intermediate.
   For larger seq_t (arbitrary‑precision) we rely on lib operators. */
static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod)
{
    if (sizeof(seq_t) <= sizeof(uint64_t))
    {
        return ((unsigned __int128)a * b) % mod;
    }
    else
    {
        return (a * b) % mod;
    }
}

/* Fast doubling Fibonacci modulo mod: returns (F(n), F(n+1)). */
static void fib_mod(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1)
{
    if (n == 0)
    {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_mod(n >> 1, mod, &a, &b);      /* a = F(k), b = F(k+1) */

    /* c = F(2k) = F(k) * (2*F(k+1) – F(k)) */
    seq_t two_b = mul_mod((seq_t)2, b, mod);
    seq_t t = (two_b >= a) ? (two_b - a) : (two_b + mod - a);
    seq_t c = mul_mod(a, t, mod);

    /* d = F(2k+1) = F(k)^2 + F(k+1)^2 */
    seq_t aa = mul_mod(a, a, mod);
    seq_t bb = mul_mod(b, b, mod);
    seq_t d = aa + bb;
    if (d >= mod) d -= mod;

    if (n & 1)
    {
        *fn  = d;
        seq_t sum = c + d;
        if (sum >= mod) sum -= mod;
        *fn1 = sum;
    }
    else
    {
        *fn  = c;
        *fn1 = d;
    }
}

/* Lucas(k) modulo mod.  Lucas(k) = 2*F(k+1) – F(k). */
static inline seq_t lucas_mod(idx_t k, seq_t mod)
{
    seq_t f_k, f_kp1;
    fib_mod(k, mod, &f_k, &f_kp1);
    seq_t two_fkp1 = mul_mod((seq_t)2, f_kp1, mod);
    seq_t luc = (two_fkp1 >= f_k) ? (two_fkp1 - f_k) : (two_fkp1 + mod - f_k);
    return luc;
}

/* Memoized sequence constructor. */
seq_t a(idx_t n)
{
    /* Static state for caching discovered terms. */
    static seq_t *terms = NULL;
    static size_t terms_size = 0;
    static size_t terms_cap   = 0;
    static uint64_t last_prime = 2;   /* prime to start searching from */

    /* Ensure the cache holds at least n terms. */
    while (terms_size < n)
    {
        /* Find next prime after the last one examined. */
        last_prime = next_prime(last_prime);

        /* Compute Lucas((p-1)/2) modulo p. */
        idx_t k = ( (idx_t)last_prime - 1 ) >> 1;   /* (p-1)/2 */
        seq_t luc = lucas_mod(k, (seq_t)last_prime);

        /* If the prime divides the Lucas number, store it. */
        if (luc == 0)
        {
            if (terms_size == terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                seq_t *tmp = alloc_seq_array(new_cap);
                if (terms)
                    memcpy(tmp, terms, terms_size * sizeof(seq_t));
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_size++] = (seq_t)last_prime;
        }
    }

    /* Return the requested term (1‑based indexing). */
    return terms[n - 1];
}
