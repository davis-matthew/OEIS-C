/*
 * A051082  Primes p such that x^26 = -2 has a solution mod p.
 * https://oeis.org/A051082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_u64, int_grow, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A051082: Prime p such that x^26 = -2 has a solution mod p.
   Implementation: iterate over odd primes, test condition
   (-2)^( (p-1)/gcd(26,p-1) ) ≡ 1 (mod p). 
   2 is always included (x=0 solves x^26 = -2 mod 2).
*/
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"   /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/gcd_u64.h"
#include "../utils/int_grow.h"

/* gcd for 64‑bit integers */

/* Dynamic list of 64‑bit primes for primality testing */
struct prime_list {
    uint64_t *p;
    size_t len, cap;
} primes = {NULL, 0, 0};

/* Dynamic list of sequence values */
struct seq_list {
    seq_t *v;
    size_t len, cap;
} vals = {NULL, 0, 0};

/* Next integer to be tested for primality */
static uint64_t candidate = 2;

/* Simple primality test using stored primes */
static bool is_prime_candidate(uint64_t n)
{
    if (n < 2) return false;
    uint64_t r = (uint64_t)sqrt((double)n);
    for (size_t i = 0; i < primes.len; ++i) {
        uint64_t p = primes.p[i];
        if (p > r) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Append prime to list */
static void add_prime(uint64_t p)
{
    if (primes.len == primes.cap) int_grow(&primes.cap, (void**)&primes.p, sizeof(uint64_t));
    primes.p[primes.len++] = p;
}

/* Append value to sequence list */
static void add_value(seq_t v)
{
    if (vals.len == vals.cap) int_grow(&vals.cap, (void**)&vals.v, sizeof(seq_t));
    vals.v[vals.len++] = v;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure we have at least n entries */
    while (vals.len < (size_t)n) {
        /* find next prime */
        while (!is_prime_candidate(candidate))
            ++candidate;
        uint64_t p = candidate;
        add_prime(p);
        /* test condition for odd primes */
        if (p == 2) {
            add_value((seq_t)2);
        } else {
            uint64_t d = gcd_u64(26ULL, p - 1ULL);
            uint64_t exp = (p - 1ULL) / d;
            seq_t base = (seq_t)(p - 2ULL);          /* -2 mod p */
            seq_t res  = mod_pow(base, exp, (seq_t)p);
            if (res == (seq_t)1) add_value((seq_t)p);
        }
        ++candidate;
    }
    return vals.v[n - 1];
}
