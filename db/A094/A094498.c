/*
 * A094498  Least prime factor of 2^(4*n) + 3^(4*n) = 16^n + 81^n.
 * https://oeis.org/A094498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 255, u64 n <= 255,
 *            u128 n <= 255, big any n
 * Validated: standard, pass, 66 terms through n = 240 (2026-11-03)
 * Helpers:   build_primes, is_prime_u64, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least prime factor of 16^n + 81^n = 2^(4*n) + 3^(4*n) */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes.h"
#include "../utils/powmod.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
        seq_t r16 = powmod(base16, n, (seq_t)p);
        seq_t r81 = powmod(base81, n, (seq_t)p);
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
            seq_t r16 = powmod(base16, n, (seq_t)candidate);
            seq_t r81 = powmod(base81, n, (seq_t)candidate);
            seq_t sum = r16 + r81;
            if (sum >= (seq_t)candidate) sum -= (seq_t)candidate;
            if (sum == 0) return (seq_t)candidate;
        }
        candidate += 2;   /* skip even numbers */
    }
}
