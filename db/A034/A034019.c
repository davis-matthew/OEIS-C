/*
 * A034019  Numbers that are imprimitively (and possibly also primitively)
 *          represented by x^2+xy+y^2.
 * https://oeis.org/A034019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 5865, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A034019 – Numbers that are imprimitive (or also primitive) represented by x^2+xy+y^2.
   The sequence consists of all positive integers n that can be written as
   n = x^2 + xy + y^2 for some integers x,y with gcd(x,y) > 1.
   An integer n has such a representation iff
     (i) every prime p ≡ 2 (mod 3) appears in the prime factorisation of n with an even exponent,
     (ii) n is divisible by a non‑trivial square (i.e. it has at least one prime factor with exponent ≥ 2).
   The routine below generates the sequence by testing each integer in turn, storing the results in a
   dynamically resised array so that subsequent calls can be answered in O(1) time. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"

static bool initialized = false;
static idx_t *prime_list = NULL;
static size_t prime_count = 0;

/* Initialise the list of primes once (up to 1 000 000, sufficient for n up to ~10⁵). */
static void init_primes(void)
{
    if (!initialized) {
        /* Build primes with the helper; we ignore the returned memory limits for safety. */
        build_primes( (idx_t)1000000, &prime_list, &prime_count );
        initialized = true;
    }
}

/* Return true iff n has an imprimitive representation by x^2+xy+y^2. */
static bool has_imprimitive_rep(uint64_t n)
{
    if (n <= 1)
        return false;

    bool has_square = false;
    uint64_t temp = n;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        /* stop once p^2 > temp */
        if ( (__uint128_t)p * p > temp )
            break;

        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (p % 3 == 2 && (exp & 1))
                return false;                     /* odd exponent of a 2 mod 3 prime */
            if (exp >= 2)
                has_square = true;                /* found a squared factor */
        }
    }

    /* Remaining factor (if any) is a prime > sqrt(n) */
    if (temp > 1) {
        uint64_t p = temp;                       /* exponent is 1 */
        if (p % 3 == 2)
            return false;                       /* odd exponent of a 2 mod 3 prime */
        /* exponent 1 cannot contribute a square */
    }

    return has_square;                           /* must have a non‑trivial square divisor */
}

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t capacity = 0;
    static idx_t count = 0;
    static uint64_t candidate = 1;               /* next integer to test */

    init_primes();

    while (count < n) {
        if (has_imprimitive_rep(candidate)) {
            if (count >= capacity) {
                capacity = capacity ? capacity * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq, capacity * sizeof(seq_t));
                if (!tmp) exit(1);
                seq = tmp;
            }
            seq[count++] = (seq_t)candidate;
        }
        ++candidate;
    }
    return seq[n - 1];
}
