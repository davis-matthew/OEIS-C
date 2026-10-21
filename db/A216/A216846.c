/*
 * A216846  Union of the composite numbers and the primes for which 2 is a
 *          primitive root.
 * https://oeis.org/A216846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer ≥ 3 that is either composite or a prime for
 *            which 2 is a primitive root modulo that prime
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   build_primes_upto, is_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/powmod64.h"

/* Global list of primes used for factorisation.  It is built once
   up to a limit that comfortably exceeds the square root of any value
   encountered for n up to about 10 000. */
static uint64_t *prime_list = NULL;
static size_t     prime_count = 0;

static void init_primes(void)
{
    if (!prime_list) {
        /* 2 000 000 is far above the needed bound (sqrt(2 000 000) ≈ 1 414). */
        idx_t limit = 2000000;
        build_primes_upto(limit, &prime_list, &prime_count);
    }
}

/*  Test whether 2 has order p‑1 modulo the prime p (p > 2). */
static bool is_primitive_root2_prime(uint64_t p)
{
    uint64_t m = p - 1;          /* p-1 is the group order */
    uint64_t temp = m;

    /* Iterate over the pre‑computed prime table to obtain the distinct
       prime divisors of m.  Only distinct divisors are needed. */
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= temp; ++i) {
        uint64_t q = prime_list[i];
        if (temp % q == 0) {
            /* q is a prime divisor of m.  If 2^(m/q) ≡ 1 (mod p), p is not a generator. */
            if (powmod64(2, m / q, p) == 1) {
                return false;
            }
            while (temp % q == 0) {
                temp /= q;
            }
        }
    }
    /* If a factor larger than sqrt(m) remains, it must be prime. */
    if (temp > 1) {
        if (powmod64(2, m / temp, p) == 1) {
            return false;
        }
    }
    return true;
}

/*  Main sequence function. */
seq_t a(idx_t n)
{
    init_primes();            /* make sure the prime table is ready */
    idx_t seen = 0;           /* number of terms already found */
    uint64_t cand = 3;        /* smallest candidate (a(1) = 3) */

    for (;;) {
        if (!is_prime(cand)) {
            /* composite numbers are always in the sequence */
            seen++;
        } else if (is_primitive_root2_prime(cand)) {
            /* prime with 2 as primitive root */
            seen++;
        }
        if (seen == n) {
            return (seq_t)cand;
        }
        cand++;
    }
}
