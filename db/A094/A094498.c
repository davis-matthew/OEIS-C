/*
 * A094498  Least prime factor of 2^(4*n) + 3^(4*n) = 16^n + 81^n.
 * https://oeis.org/A094498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 255, u64 n <= 255,
 *            u128 n <= 255, big any n
 * Helpers:   build_primes, is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least prime factor of 16^n + 81^n = 2^(4*n) + 3^(4*n) */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/mod_pow.h"      /* seq_t mod_pow(seq_t base, idx_t exp, seq_t mod); */
#include "../utils/build_primes.h"  /* void build_primes(idx_t limit, idx_t **out, size_t *cnt); */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    /* Static cache of small primes */
    static uint64_t *small_primes = NULL;
    static size_t     prime_cnt   = 0;
    static bool       primes_ready = false;
    static const uint64_t PRIME_LIMIT = 1000000;   /* primes up to 1 000 000 */

    /* One‑time initialization of the small prime list */
    if (!primes_ready) {
        build_primes(PRIME_LIMIT, (idx_t **)&small_primes, &prime_cnt);
        primes_ready = true;
    }

    /* Modular pre‑computations */
    const seq_t base16 = (seq_t)16;
    const seq_t base81 = (seq_t)81;

    /* Test all precomputed small primes */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = small_primes[i];
        seq_t r16 = mod_pow(base16, n, (seq_t)p);
        seq_t r81 = mod_pow(base81, n, (seq_t)p);
        seq_t sum = r16 + r81;
        if (sum >= (seq_t)p) sum -= (seq_t)p;
        if (sum == 0) return (seq_t)p;
    }

    /* If no divisor was found among the precomputed primes, test larger candidates */
    /* Start scanning from the first odd number after the largest precomputed prime */
    uint64_t candidate = small_primes[prime_cnt - 1] + 2;
    if ((candidate & 1ULL) == 0) ++candidate;   /* make sure it is odd */

    for (;;) {
        if (is_prime_big((seq_t)candidate)) {
            seq_t r16 = mod_pow(base16, n, (seq_t)candidate);
            seq_t r81 = mod_pow(base81, n, (seq_t)candidate);
            seq_t sum = r16 + r81;
            if (sum >= (seq_t)candidate) sum -= (seq_t)candidate;
            if (sum == 0) return (seq_t)candidate;
        }
        candidate += 2;   /* skip even numbers */
    }
}
