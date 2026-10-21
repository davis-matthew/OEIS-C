/*
 * A059349  Primes p such that x^32 = 2 has no solution mod p.
 * https://oeis.org/A059349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   modpow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A059349
 *  a(n) = the n-th prime p such that x^32 ≡ 2 (mod p) has no solution
 *
 *  The congruence has a solution iff 2^{(p-1)/gcd(32,p-1)} ≡ 1 (mod p).
 *  Thus we test each prime p (p>2) and count those with
 *      2^{(p-1)/gcd(32,p-1)} ̸≡ 1 (mod p)
 *
 *  We sieve primes up to a fixed bound (~400 000) which is sufficient for
 *  the first 10 000 terms.  The test is O(1) for each prime.
 *
 *  The function is memoised: a static array of primes is built once and
 *  reused for all calls.  The algorithm runs in <0.01 s for n≤10 000.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/modpow_seq.h"

/* helper: binary GCD for uint64_t */
static inline uint64_t ugcd(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* sieve limit (sufficient for n up to 10 000) */
#define SIEVE_LIMIT 400000
/* maximum possible number of primes below SIEVE_LIMIT */
#define PRIME_CAP 35000

static uint32_t primes[PRIME_CAP];
static size_t primes_cnt = 0;
static int sieve_built = 0;

/* build sieve once */
static void build_sieve(void) {
    uint8_t *is_prime = (uint8_t*)malloc(SIEVE_LIMIT + 1);
    memset(is_prime, 1, SIEVE_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }
    primes_cnt = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            primes[primes_cnt++] = i;
        }
    }
    free(is_prime);
    sieve_built = 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, but safety */
    if (!sieve_built)
        build_sieve();

    size_t found = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if (p == 2) continue;     /* skip 2 – trivial solution exists */
        uint64_t d = ugcd(32ULL, (uint64_t)p - 1ULL);
        uint64_t exp = ((uint64_t)p - 1ULL) / d;
        seq_t rem = modpow_seq((seq_t)2, exp, (seq_t)p);
        if (rem != (seq_t)1) {    /* 2 is not a 32nd power residue mod p */
            ++found;
            if (found == (size_t)n)
                return (seq_t)p;
        }
    }
    /* in case the bound is insufficient (should not happen for n ≤ 10 000) */
    return 0;
}
