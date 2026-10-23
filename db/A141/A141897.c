/*
 * A141897  Primes congruent to 19 mod 21.
 * https://oeis.org/A141897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 549, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A141897: primes congruent to 19 modulo 21 (offset 1)
   The nth term is the nth prime p with p ≡ 19 (mod 21).  This
   implementation generates the needed primes on demand by testing
   consecutive numbers of the form 19 + 21·k. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

/* cached primes ≡ 19 (mod 21) */
static seq_t *prime_cache = NULL;   /* array of found primes */
static size_t primed_cnt = 0;       /* number of stored primes */
static size_t primed_cap = 0;       /* capacity of prime_cache */

/* next candidate to test for primality */
static idx_t candidate = 19;        /* start from 19 */

/* enlarge cache when needed */
static void ensure_cache_cap(size_t needed)
{
    if (primed_cap >= needed) return;
    size_t new_cap = primed_cap ? (primed_cap << 1) : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();              /* out of memory */
    prime_cache = tmp;
    primed_cap = new_cap;
}

/* generate primes ≡ 19 (mod 21) until at least n primes are stored */
static void generate_up_to(idx_t n)
{
    while (primed_cnt < n) {
        if (is_prime(candidate)) {            /* test only numbers ≡ 19 mod 21 */
            ensure_cache_cap(primed_cnt + 1);
            prime_cache[primed_cnt++] = (seq_t)candidate;
        }
        candidate += 21;                       /* next candidate of the form 19+21·k */
    }
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n<1, return 0 */
    generate_up_to(n);
    return prime_cache[n - 1];
}
