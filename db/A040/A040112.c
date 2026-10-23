/*
 * A040112  Primes p such that x^4 = 7 has a solution mod p.
 * https://oeis.org/A040112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   list of primes p such that x^4 ≡ 7 (mod p) has a solution
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/seq_realloc.h"

/* Test whether a prime p satisfies that 7 is a fourth‑power residue modulo p. */
static bool good_prime_condition(uint64_t p)
{
    /* 2 always works (7 ≡ 1 mod 2). */
    if (p == 2ULL) return true;

    /* p = 7 is a special case: x = 0 is a solution. */
    if (p == 7ULL) return true;

    /* Compute the exponent e = (p-1)/gcd(p-1,4) */
    uint64_t p1 = p - 1ULL;
    uint64_t e;
    /* gcd(p-1,4) is 4 if p≡1 (mod 4), otherwise 2 */
    if ((p1 & 3ULL) == 0ULL) {
        e = p1 >> 2;          /* (p-1)/4 */
    } else {
        e = p1 >> 1;          /* (p-1)/2 */
    }

    seq_t r = mod_pow((seq_t)7, e, (seq_t)p);
    return r == (seq_t)1;
}

/* Cache of good primes found so far */
static seq_t *good_primes   = NULL;
static size_t good_len      = 0;
static size_t good_cap      = 0;
static uint64_t last_checked = 1;   /* last integer examined */

/* Grow the cache to at least 'need' elements. */
static void grow_good_primes(size_t need)
{
    if (good_cap >= need) return;
    size_t new_cap = good_cap ? good_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = seq_realloc(good_primes, good_cap, new_cap);
    if (!tmp) abort();                    /* out of memory */
    good_primes = tmp;
    good_cap    = new_cap;
}

/* Return the n-th prime with the desired property. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined, but for safety */

    /* Ensure we have at least n good primes. */
    while (good_len < (size_t)n) {
        ++last_checked;
        if (!is_prime((idx_t)last_checked)) continue;
        if (good_prime_condition(last_checked)) {
            grow_good_primes(good_len + 1);
            good_primes[good_len++] = (seq_t)last_checked;
        }
    }
    return good_primes[n - 1];
}
