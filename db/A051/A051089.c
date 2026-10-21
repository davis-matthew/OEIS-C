/*
 * A051089  Primes p such that x^40 = -2 has a solution mod p.
 * https://oeis.org/A051089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime p such that the congruence x^40 ≡ -2 (mod
 *            p) has a solution. For the odd prime p this happens iff
 *            (-2)^((p-1)/gcd(40,p-1)) ≡ 1 (mod p). The prime p=2 trivially
 *            satisfies the congruence. The sequence is obtained by generating
 *            primes in ascending order and keeping those that...
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include necessary headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"

/* Helper: greatest common divisor for 64-bit values */

/* Helper: modular exponentiation (base^exp mod mod) */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* Main function */
seq_t a(idx_t n) {
    /* Static state for the sequence */
    static seq_t *seq = NULL;          /* array of found terms */
    static size_t seq_len  = 0;        /* current number of terms stored */
    static size_t seq_cap  = 0;        /* capacity of seq array */

    static uint64_t *prime_list = NULL;/* list of primes encountered */
    static size_t prime_len  = 0;      /* number of primes stored */
    static size_t prime_cap  = 0;      /* capacity of prime_list */

    /* Current candidate to test for primality */
    static uint64_t candidate = 2;

    /* Ensure we have enough terms for requested n */
    while (seq_len < n) {
        /* Check if candidate is prime (trial division by known primes) */
        bool is_prime = true;
        for (size_t i = 0; i < prime_len && prime_list[i] * prime_list[i] <= candidate; ++i) {
            if (candidate % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        /* If prime, test the residue condition */
        if (is_prime) {
            /* Add candidate to prime list */
            if (prime_len >= prime_cap) {
                size_t newcap = prime_cap == 0 ? 16 : prime_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, newcap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                prime_list = tmp;
                prime_cap = newcap;
            }
            prime_list[prime_len++] = candidate;

            bool satisfies = false;
            if (candidate == 2) {
                satisfies = true;               /* x^40 ≡ -2 mod 2 has solution x≡0 */
            } else {
                uint64_t d   = gcd_uint64(40ULL, candidate - 1);
                uint64_t exp = (candidate - 1) / d;
                uint64_t base = (candidate + candidate - 2) % candidate; /* -2 mod candidate */
                satisfies = mod_pow_uint64(base, exp, candidate) == 1;
            }

            if (satisfies) {
                /* Store term in sequence array */
                if (seq_len >= seq_cap) {
                    size_t newcap = seq_cap == 0 ? 16 : seq_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    seq = tmp;
                    seq_cap = newcap;
                }
                seq[seq_len++] = (seq_t)candidate;
            }
        }

        /* Advance to next candidate, skipping even numbers >2 */
        if (candidate == 2) candidate = 3;
        else candidate += 2;
    }

    /* Return the requested term (offset 1) */
    return seq[n - 1];
}
