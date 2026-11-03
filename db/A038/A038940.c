/*
 * A038940  Primes p such that 59 is not a square mod p.
 * https://oeis.org/A038940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 861 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038940: primes p for which 59 is not a quadratic residue mod p */

/* Helper: generate primes up to a given limit using a simple sieve.
 * The primes are stored in static arrays to allow subsequent calls to reuse
 * the computed table. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Test whether 59 is NOT a quadratic residue modulo prime p. */
static bool not_square_prime(uint32_t p) {
    if (p == 2)                return true;   /* explicit inclusion */
    if (p == 59)               return false;  /* 59 divides 59 → 0 is a square */

    if (p < 59) {               /* brute‑force Legendre for small primes */
        uint32_t s = 59 % p;
        uint64_t result = 1;
        uint64_t base   = s;
        uint64_t exp    = (p - 1) / 2;
        uint64_t mod    = p;
        while (exp) {
            if (exp & 1) result = (result * base) % mod;
            base = (base * base) % mod;
            exp >>= 1;
        }
        return result == mod - 1;   /* -1 Legendre symbol → non‑residue */
    }

    /* For p > 59 use quadratic reciprocity.  (59/p) = (p/59)*(-1)^((p-1)/2) */
    static bool residues[59] = {0};
    static bool initialized = false;
    if (!initialized) {
        for (uint32_t i = 1; i < 59; ++i)
            residues[(i * i) % 59] = true;
        initialized = true;
    }

    uint32_t r = p % 59;
    bool is_qr = residues[r];            /* (p/59) = +1 if true */

    if (p % 4 == 1)                      /* sign = +1 */
        return !is_qr;                   /* include when (p/59) = -1 */

    /* p % 4 == 3, sign = -1 */
    return is_qr;                        /* include when (p/59) = +1 */
}

static uint32_t *primes = NULL;          /* list of primes */
static size_t prime_cnt = 0;             /* number of primes stored */
static size_t prime_limit = 0;           /* upper bound of the table */

static bool *is_comp = NULL;             /* composite marks for current sieve */

/* Build or extend the prime table up to 'limit' (inclusive). */
static void build_primes_up_to(size_t limit) {
    if (limit <= prime_limit) return;   /* already sufficient */

    /* free old sieve array */
    free(is_comp);
    is_comp = (bool*)calloc(limit + 1, 1);
    if (!is_comp) {                     /* allocation failure */
        prime_cnt = 0;
        prime_limit = limit;
        return;
    }
    is_comp[0] = is_comp[1] = true;

    /* sieve */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;
        }
    }

    /* collect primes */
    free(primes);
    primes = (uint32_t*)malloc(limit * sizeof(uint32_t)); /* overshoot but OK */
    if (!primes) {
        prime_cnt = 0;
        prime_limit = limit;
        return;
    }
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) primes[cnt++] = (uint32_t)i;

    prime_cnt = cnt;
    prime_limit = limit;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset = 1 – behaviour for n<1 undefined */

    size_t limit = (size_t)n * 20 + 100;  /* generous initial bound */

    while (true) {
        build_primes_up_to(limit);
        size_t found = 0;

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = primes[i];
            if (!not_square_prime(p)) continue;
            ++found;
            if (found == n)
                return (seq_t)p;
        }
        /* Not enough terms – increase the sieve limit and try again */
        limit *= 2;
    }
}
