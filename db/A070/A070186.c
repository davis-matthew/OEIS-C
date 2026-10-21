/*
 * A070186  Primes p such that x^10 = 2 has a solution mod p, but x^(10^2) = 2
 *          has no solution mod p.
 * https://oeis.org/A070186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   ensure_capacity, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A070186: primes p such that 2 is a 10th power mod p but not a 100th power */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/ensure_capacity.h"
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static seq_t *valid_primes = NULL;   /* array of found primes   */
static size_t   val_len   = 0;        /* number of primes stored */
static size_t   val_cap   = 0;        /* current capacity of array*/
static uint64_t next_candidate = 3;   /* next odd number to test   */

static void append_prime(seq_t prime)
{
    if (val_len + 1 > val_cap) {
        ensure_capacity(val_len + 1, &val_cap, (void **)&valid_primes,
                        sizeof(seq_t));
    }
    valid_primes[val_len++] = prime;
}

/* ------------------------------------------------------------------ */
/*  a(idx_t n) – the nth prime p for which   2 is a 10th power mod p   */
/*  but 2 is NOT a 100th power mod p.                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0u) {           /* offset starts at 1 – undefined for 0   */
        return (seq_t)0;
    }

    while (val_len < (size_t)n) {
        /* Find next prime candidate */
        uint64_t p;
        do {
            p = next_candidate;
            next_candidate += 2;           /* keep odds only          */
        } while (!is_prime(p));

        /* Check the congruence conditions for p */
        uint64_t d = p - 1ULL;
        uint64_t g10  = gcd_u64(10ULL, d);
        uint64_t g100 = gcd_u64(100ULL, d);
        uint64_t exp10  = d / g10;
        uint64_t exp100 = d / g100;

        seq_t mod = (seq_t)p;
        seq_t pow10 = mod_pow((seq_t)2, exp10, mod);
        if (pow10 != (seq_t)1) {
            continue; /* 2 is NOT a 10th power mod p */
        }

        seq_t pow100 = mod_pow((seq_t)2, exp100, mod);
        if (pow100 == (seq_t)1) {
            continue; /* 2 IS a 100th power mod p – reject */
        }

        /* p satisfies the required property; store it */
        append_prime(mod);
    }

    return valid_primes[(size_t)n - 1u];
}
