/*
 * A239441  Number of invertible octonions over Z/nZ.
 * https://oeis.org/A239441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e dividing n of f(p^e) where
 *            f(2^e) = 2^(8e-1) f(p^e) = (p-1) * p^(8e-5) * (p^4-1) for odd p
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 256, u128 n <= 1000,
 *            big any n
 * Helpers:   build_primes_upto, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative arithmetic function for A239441. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes_upto.h"   /* builds primes up to a limit */
#include "../utils/seq_pow.h"             /* seq_pow(base, exp) */

static uint64_t *primes = NULL;
static size_t prime_count = 0;
static bool primes_built = false;

/* Ensure a prime list large enough for factorisation of n up to at least
   100,000 (more than enough for n <= 10,000). */
static void ensure_primes(void) {
    if (!primes_built) {
        build_primes_upto(100000, &primes, &prime_count);
        primes_built = true;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;             /* invalid input, but safe */
    ensure_primes();

    idx_t temp = n;
    seq_t result = 1;                 /* multiplicative identity */

    /* factor out primes one by one */
    for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p != 0) continue;
        int e = 0;
        while (temp % p == 0) {
            ++e;
            temp /= p;
        }

        /* compute f(p^e) */
        seq_t local;
        if (p == 2) {
            int exp = 8 * e - 1;
            local = seq_pow((seq_t)2, (idx_t)exp);
        } else {
            seq_t p_val = (seq_t)p;
            seq_t term1 = (seq_t)(p - 1);
            seq_t term2 = seq_pow(p_val, (idx_t)(8 * e - 5));
            seq_t term3 = seq_pow(p_val, (idx_t)4) - 1;
            local = term1 * term2 * term3;
        }
        result *= local;
    }

    /* remaining prime factor if any */
    if (temp > 1) {
        uint64_t p = temp;
        int e = 1;
        seq_t local;
        if (p == 2) {
            int exp = 8 * e - 1;          /* e == 1 */
            local = seq_pow((seq_t)2, (idx_t)exp);
        } else {
            seq_t p_val = (seq_t)p;
            seq_t term1 = (seq_t)(p - 1);
            seq_t term2 = seq_pow(p_val, (idx_t)(8 * e - 5));   /* (8*1-5) = 3 */
            seq_t term3 = seq_pow(p_val, (idx_t)4) - 1;
            local = term1 * term2 * term3;
        }
        result *= local;
    }

    return result;
}
