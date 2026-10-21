/*
 * A051696  Greatest common divisor of n! and n^n.
 * https://oeis.org/A051696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = product_{p|n} p^(sum_{k>=1} floor(n/p^k))
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 23, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* power of a base (seq_t) raised to an exponent (idx_t) using
   repeated squaring.  Works for any seq_t type. */
static seq_t pow_seq_t(idx_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    seq_t b = (seq_t)base;
    while (exp) {
        if (exp & 1) {
            result *= b;
        }
        b *= b;
        exp >>= 1;
    }
    return result;
}

/* sieve for primes up to 20000 – enough for n ≤ 10000 (and many more). */
static bool sieve_initialized = false;
static bool sieve[20001];
static uint32_t primes[1300];
static size_t prime_cnt = 0;

static void init_sieve(void) {
    if (sieve_initialized) return;
    const idx_t MAX_N = 20000;
    for (idx_t i = 0; i <= MAX_N; ++i) sieve[i] = false;
    for (idx_t i = 2; i <= MAX_N; ++i) {
        if (!sieve[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            if (i * i <= MAX_N) {
                for (idx_t j = i * i; j <= MAX_N; j += i) sieve[j] = true;
            }
        }
    }
    sieve_initialized = true;
}

seq_t a(idx_t n) {
    init_sieve();
    if (n <= 1) return (seq_t)1;   /* a(1) = 1 */

    seq_t result = (seq_t)1;

    /* iterate over primes ≤ n */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((idx_t)p > n) break;

        /* prime p contributes only if it divides n */
        if (n % p == 0) {
            /* exponent of p in n! : sum_{k>=1} floor(n/p^k) */
            idx_t exp = 0;
            idx_t t = n;
            while (t) {
                t /= p;
                exp += t;
            }
            result *= pow_seq_t((idx_t)p, exp);
        }
    }
    return result;
}
