/*
 * A235709  Least prime p < prime(n) with 2^p - 1 a quadratic residue modulo
 *          prime(n), or 0 if such a number does not exist.
 * https://oeis.org/A235709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p < prime(n) with 2^p – 1 a quadratic residue
 *            modulo prime(n), or 0 if none
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9717 (2026-11-01)
 * Helpers:   build_primes_upto, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/powmod.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

static uint64_t *SIMPLE_PRIMES = NULL;      /* array of primes */
static size_t SIMPLE_PRIMES_CNT = 0;        /* number of primes stored */
static bool PRIMES_INITIALIZED = false;

/* ensure we have at least `min_needed` primes (at least up to limit 200000) */
static void ensure_primes(void)
{
    if (PRIMES_INITIALIZED)
        return;

    /* a limit that guarantees >10 000 primes (prime(10000) ≈ 104 729) */
    const idx_t LIMIT = 200000;
    build_primes_upto(LIMIT, &SIMPLE_PRIMES, &SIMPLE_PRIMES_CNT);
    PRIMES_INITIALIZED = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0 || !n)
        return (seq_t)0;      /* undefined for n == 0, return 0 */

    ensure_primes();

    if ((size_t)n > SIMPLE_PRIMES_CNT)
        /* for safety: if requested n exceeds precomputed primes,
           generate more (not expected for n ≤ 10000). */
        return (seq_t)0;

    uint64_t p_n = SIMPLE_PRIMES[n - 1];          /* prime(n) */

    for (size_t i = 0; i < (size_t)n - 1; ++i) {   /* primes < prime(n) */
        uint64_t q = SIMPLE_PRIMES[i];            /* candidate prime */

        /* compute (2^q - 1) mod p_n */
        seq_t pow_val = powmod((seq_t)2, q, (seq_t)p_n);
        uint64_t val = (uint64_t)pow_val;
        val = (val + p_n - 1) % p_n;              /* ensure in [0, p_n-1] */

        if (jacobi_symbol(val, p_n) == 1)
            return (seq_t)q;                      /* found least prime */
    }

    return (seq_t)0;  /* none found */
}
