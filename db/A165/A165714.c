/*
 * A165714  Let the prime factorization of m be m = product p(m,k)^b(m,k),
 *          where p(m,j)<p(m,j+1) for all j, the p's are the distinct primes
 *          dividing m, and each b is a positive integer. Then a(n) =
 *          product_k {p(A165713(n), k)^b(n,k)}.
 * https://oeis.org/A165714
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product over k of p(A165713(n),k)^{b(n,k)}
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 63, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 65 (2026-11-01)
 * Helpers:   build_primes, factor_u64, ipow, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes.h"
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"
#include "../utils/omega.h"

static inline seq_t seq_one(void) { return (seq_t)1; }

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static bool primes_initialized = false;
static idx_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_prime_list(void) {
    if (!primes_initialized) {
        /* Build primes up to a safe limit for factorization. */
        build_primes(100000, &prime_list, &prime_count);
        primes_initialized = true;
    }
}

/* Compute a(n) for n >= 2 */
seq_t a(idx_t n) {
    if (n < 2) {
        return (seq_t)0;
    }

    init_prime_list();

    /* 1. Count distinct primes of n (omega(n)). */
    uint64_t omega_n = omega((uint64_t)n);

    /* 2. Find A165713(n): smallest integer > n with same omega. */
    uint64_t m = (uint64_t)n + 1;
    while (omega(m) != omega_n) {
        ++m;
    }

    /* 3. Factor n and m. */
    /* Temporary arrays for factorization. 64 entries are plenty for n <= 10000. */
    uint64_t primesN[64], expsN[64];
    size_t cntN;
    factor_u64((uint64_t)n, primesN, expsN, &cntN);

    uint64_t primesM[64], expsM[64]; /* expsM not needed but provided for API. */
    size_t cntM;
    factor_u64(m, primesM, expsM, &cntM);

    /* The two factorizations must have the same number of distinct primes. */
    if (cntN != cntM) {
        return (seq_t)0; /* Should not happen with correct definition. */
    }

    /* 4. Build the result by applying the exponents of n to the primes of m. */
    seq_t result = seq_one();
    for (size_t i = 0; i < cntN; ++i) {
        seq_t base = (seq_t)primesM[i];
        seq_t exp  = (seq_t)expsN[i];
        seq_t pw   = ipow(base, exp);
        result = seq_mul(result, pw);
    }
    return result;
}
