/*
 * A051080  Primes p such that x^22 = -2 has a solution mod p.
 * https://oeis.org/A051080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   nth prime p for which the congruence x^22 ≡ -2 (mod p) has a
 *            solution
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/is_prime.h"

/* Static state for memoisation */
static idx_t   next_candidate = 2;   /* next integer to test */
static seq_t*  good_arr       = NULL;/* array of qualifying primes */
static size_t  good_cnt       = 0;    /* number of primes found */
static size_t  good_cap       = 0;    /* allocated capacity */

/* Test whether a prime p satisfies x^22 ≡ -2 mod p */
static bool is_good_prime(seq_t p)
{
    seq_t g   = gcd(p - 1, (seq_t)22);         /* gcd(p-1,22)  */
    seq_t exp = (p - 1) / g;                  /* (p-1)/g      */
    seq_t base = p - 2;                       /* -2 mod p      */
    seq_t r   = mod_pow(base, (uint64_t)exp, p);
    return r == 1;
}

/* Append a prime to the dynamic array, expanding it if needed */
static void append_prime(seq_t p)
{
    if (good_cnt + 1 > good_cap) {
        size_t new_cap = good_cap ? good_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(good_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        good_arr = tmp;
        good_cap = new_cap;
    }
    good_arr[good_cnt++] = p;
}

/* Public function: returns the nth term (1‑based) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n=0 */

    while (good_cnt < n) {
        idx_t p = next_candidate;

        /* Handle the special case p = 2 */
        if (p == 2) {
            append_prime((seq_t)2);
            next_candidate = 3;
            continue;
        }

        /* Test odd candidate only */
        if (is_prime(p) && is_good_prime((seq_t)p))
            append_prime((seq_t)p);

        /* Advance to next odd number */
        next_candidate += 2;
    }

    return good_arr[n - 1];
}
