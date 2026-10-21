/*
 * A059770  First solution of x^2 = 2 mod p for primes p such that a solution
 *          exists.
 * https://oeis.org/A059770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest non‑negative integer x such that x² ≡ * 2
 *            (mod p), where p is the n-th prime for which 2 is a quadratic *
 *            residue (i.e., p = 2 or p ≡ ±1 (mod 8))
 * Range:     u8 n <= 25, u16 n <= 3118, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   jacobi_symbol, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The routine searches primes incrementally, applying the Tonelli–Shanks
 *  algorithm to find the least square root of 2 modulo each qualifying
 *  prime.  Results are memoised so repeated calls with any order of n
 *  are efficient. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/mod_pow.h"

/*  Helper: modular multiplication that stays within one word for small
 *  seq_t, otherwise relies on the built‑in operator.  The cast to seq_t
 *  ensures the result has the requested type.  */
static seq_t mul_mod(seq_t a, seq_t b, seq_t mod)
{
    if (sizeof(seq_t) <= sizeof(uint16_t)) {
        uint32_t prod = (uint32_t)(uint16_t)a * (uint32_t)(uint16_t)b;
        return (seq_t)(prod % (uint32_t)(uint16_t)mod);
    }
    if (sizeof(seq_t) <= sizeof(uint32_t)) {
        uint64_t prod = (uint64_t)(uint32_t)a * (uint64_t)(uint32_t)b;
        return (seq_t)(prod % (uint64_t)(uint32_t)mod);
    }
    return (a * b) % mod;
}

/*  Global state used by the routine.  These are static so that the
 *  function is thread‑safe only with respect to a single external
 *  call – the problem specification does not require true
 *  multithreading.  */
static seq_t *s_solutions = NULL;      /* a(1)..a(s_sol_len-1)   */
static size_t   s_sol_cap   = 0;
static size_t   s_sol_len   = 0;

static uint64_t *s_primes = NULL;      /* all primes found so far */
static size_t   s_prime_cap  = 0;
static size_t   s_prime_len  = 0;

static uint64_t s_candidate = 2;       /* next integer to test for primality */

/*  Primality test for 64‑bit numbers using trial division by primes
 *  already discovered.  Only used for numbers that fit into uint64_t. */
static bool is_prime_uint64(uint64_t num)
{
    if (num < 2) return false;
    for (size_t i = 0; i < s_prime_len; ++i) {
        uint64_t p = s_primes[i];
        if (p * p > num) break;
        if (num % p == 0) return false;
    }
    return true;
}

/*  Ensure that solutions for the first n terms have been computed. */
static void ensure_solutions(idx_t n)
{
    if (s_sol_len >= n) return;   /* already available */

    /* Initialise storage on first use. */
    if (s_sol_cap == 0) {
        s_sol_cap = 64;
        s_solutions = (seq_t *)malloc(s_sol_cap * sizeof(seq_t));
    }
    if (s_prime_cap == 0) {
        s_prime_cap = 64;
        s_primes = (uint64_t *)malloc(s_prime_cap * sizeof(uint64_t));
    }

    /* Gather primes until we have n qualifying ones. */
    while (s_sol_len < n) {
        /* Find next prime. */
        while (!is_prime_uint64(s_candidate))
            ++s_candidate;

        /* Record the prime. */
        if (s_prime_len == s_prime_cap) {
            s_prime_cap *= 2;
            s_primes = (uint64_t *)realloc(s_primes,
                                            s_prime_cap * sizeof(uint64_t));
        }
        s_primes[s_prime_len++] = s_candidate;

        /* If 2 is a quadratic residue modulo this prime, compute the
         * least square root of 2. */
        if (s_candidate == 2 || (s_candidate % 8 == 1) || (s_candidate % 8 == 7)) {
            seq_t root;
            if (s_candidate == 2) {
                root = 0;   /* for p = 2 we take x = 0 */
            } else {
                uint64_t p = s_candidate;
                /* Tonelli–Shanks for a = 2.  All intermediate values are
                 * kept modulo p, which is guaranteed to fit in uint64_t. */
                uint64_t q = p - 1;
                uint64_t s = 0;
                while ((q & 1ULL) == 0) { q >>= 1; ++s; }

                /* Find a quadratic non‑residue z. */
                uint64_t z = 2;
                while (jacobi_symbol(z, p) != -1) ++z;

                seq_t c = mod_pow((seq_t)z, q, (seq_t)p);
                seq_t x = mod_pow((seq_t)2, (q + 1) / 2, (seq_t)p);
                seq_t t = mod_pow((seq_t)2, q, (seq_t)p);
                uint64_t m = s;

                while (t != 1) {
                    seq_t temp = t;
                    uint64_t i;
                    for (i = 1; i < m; ++i) {
                        temp = mul_mod(temp, temp, (seq_t)p);
                        if (temp == 1) break;
                    }
                    uint64_t exp = 1ULL << (m - i - 1);
                    seq_t b = mod_pow(c, exp, (seq_t)p);
                    x = mul_mod(x, b, (seq_t)p);
                    t = mul_mod(t, b, (seq_t)p);
                    t = mul_mod(t, b, (seq_t)p);
                    c = mul_mod(b, b, (seq_t)p);
                    m = i;
                }

                seq_t alt = (seq_t)p - x;
                root = (alt < x) ? alt : x;
            }

            /* Store the solution. */
            if (s_sol_len == s_sol_cap) {
                s_sol_cap *= 2;
                s_solutions = (seq_t *)realloc(s_solutions,
                                               s_sol_cap * sizeof(seq_t));
            }
            s_solutions[s_sol_len++] = root;
        }

        ++s_candidate;  /* advance to the next integer */
    }
}

/*  Public API required by the test harness.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* out‑of‑range behaviour */
    ensure_solutions(n);
    return s_solutions[n - 1];
}
