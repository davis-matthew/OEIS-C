/*
 * A045368  Primes congruent to {2, 5} mod 7.
 * https://oeis.org/A045368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate the primes congruent to 2 or 5 modulo 7 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* storage for all primes found */
static uint64_t *primes     = NULL;
static size_t   primes_cnt  = 0;
static size_t   primes_cap  = 0;

/* storage for qualifying primes (2 or 5 mod 7) */
static seq_t   *qual      = NULL;
static size_t   qual_cnt  = 0;
static size_t   qual_cap  = 0;

/* next integer to test for primality */
static uint64_t last_checked = 1;

/* grow the primes array to hold at least 'needed' elements */
static void grow_primes(size_t needed)
{
    if (needed <= primes_cap)
        return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    primes = tmp;
    primes_cap = new_cap;
}

/* grow the qualifying primes array to hold at least 'needed' elements */
static void grow_qual(size_t needed)
{
    if (needed <= qual_cap)
        return;
    size_t new_cap = qual_cap ? qual_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(qual, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    qual = tmp;
    qual_cap = new_cap;
}

/* test whether x is prime using the primes already found */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2U)
        return false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > x / p)            /* avoids overflow of p*p */
            break;
        if (x % p == 0)
            return false;
    }
    return true;
}

/* return the n‑th prime that is ≡ 2 or 5 (mod 7); a(1)=first such prime */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1 */
        return (seq_t)0;

    /* generate qualifying primes until we have at least n of them */
    while (qual_cnt < n) {
        ++last_checked;
        if (is_prime_uint64(last_checked)) {
            /* store the prime itself */
            grow_primes(primes_cnt + 1);
            primes[primes_cnt++] = last_checked;

            /* check the congruence condition */
            if ((last_checked % 7U) == 2U || (last_checked % 7U) == 5U) {
                grow_qual(qual_cnt + 1);
                qual[qual_cnt++] = (seq_t)last_checked;
            }
        }
    }

    return qual[n - 1];
}
