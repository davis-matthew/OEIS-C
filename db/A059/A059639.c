/*
 * A059639  Primes p such that x^50 = 2 has no solution mod p.
 * https://oeis.org/A059639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, gcd, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function returns the n‑th prime p for which the congruence
 * x^50 ≡ 2 (mod p) has no solution.  For a prime p we compute
 *      g = gcd(50, p-1)
 *      e = (p-1)/g
 * If 2^e mod p ≠ 1 then p is included in the sequence.
 * The routine generates primes on demand and caches the
 * qualifying primes for later calls.
 */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */
#include <stdint.h>   /* uint64_t */

#include "../utils/gcd.h"        /* gcd_int() – used only for small ints */
#include "../utils/mod_pow.h"    /* mod_pow() – modular exponentiation */
#include "../utils/append_candidate.h" /* append_candidate() */
#include "../utils/is_prime.h"   /* is_prime() – primality test for uint64_t */

static seq_t *prime_vals = NULL;      /* cached qualifying primes   */
static size_t prime_cnt  = 0;          /* number stored             */
static size_t prime_cap  = 0;          /* current capacity          */
static idx_t   next_trial = 3;         /* next odd candidate to check */

/* Euclidean gcd for uint64_t values (used when 50 is small). */
static inline idx_t gcd_u64(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Compute qualifying primes until at least n are stored. */
static void build_until(idx_t n) {
    while (prime_cnt < (size_t)n) {
        if (is_prime(next_trial)) {
            idx_t p_minus_1 = next_trial - 1;
            idx_t g = gcd_u64(50, p_minus_1);
            idx_t e = p_minus_1 / g;

            seq_t pow_res = mod_pow((seq_t)2, e, (seq_t)next_trial);

            if (pow_res != (seq_t)1)
                append_candidate(&prime_vals, &prime_cnt, &prime_cap,
                                 (seq_t)next_trial);
        }
        next_trial += 2;   /* step over even numbers */
    }
}

/* Public API required by the judge. */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined, but safe guard */
    build_until(n);                 /* ensure we have n primes */
    return prime_vals[(size_t)(n - 1)];
}
