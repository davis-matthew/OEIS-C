/*
 * A062068  a(n) = d(sigma(n)), where d(k) is the number of divisors function
 *          (A000005) and sigma(k) is the sum of divisor function (A000203).
 * https://oeis.org/A062068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(σ(n)) where σ(n) is the sum of divisors function * and
 *            d(k) is the number of divisors function. The implementation *
 *            uses trial division with a precomputed prime list up to 1 000
 *            000. * It works for all n ≥ 1 and is fast for n up to at least
 *            10 000 * (≈ 10⁶ elementary operations)
 * Range:     u8 n <= 229, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required types are provided by the build system */
#include <stdbool.h>
#include <stddef.h>   /* for size_t */

#define MAX_PRIME_LIMIT 1000000UL  /* enough for n up to 10⁶ or more */

/* prime storage */
static size_t prime_cnt = 0;
static idx_t primes[MAX_PRIME_LIMIT];
static bool primes_ready = false;

/* simple sieve to fill the prime list */
static void ensure_primes(void) {
    if (primes_ready) return;
    static bool is_composite[MAX_PRIME_LIMIT + 1] = {false};
    for (idx_t i = 2; i <= MAX_PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = i;
            if (i <= MAX_PRIME_LIMIT / i) {
                for (idx_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    primes_ready = true;
}

/* compute σ(n) by prime factorisation */
static seq_t sigma_of(idx_t n) {
    seq_t result = 1;
    idx_t m = n;
    ensure_primes();
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= m; ++i) {
        idx_t p = primes[i];
        if (m % p == 0) {
            seq_t p_pow = 1;
            seq_t sum = 1;
            while (m % p == 0) {
                m /= p;
                p_pow *= (seq_t)p;
                sum += p_pow;
            }
            result *= sum;
        }
    }
    if (m > 1) {          /* remaining prime factor > sqrt(n) */
        result *= (seq_t)(1 + m);
    }
    return result;
}

/* compute d(k) by prime factorisation */
static seq_t divisor_count_of(idx_t k) {
    seq_t d = 1;
    idx_t m = k;
    ensure_primes();
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= m; ++i) {
        idx_t p = primes[i];
        if (m % p == 0) {
            seq_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp += 1;
            }
            d = d * (exp + 1);
        }
    }
    if (m > 1) {          /* remaining prime factor */
        d *= 2;           /* exponent 1 -> 1+1 = 2 */
    }
    return d;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but guard */
    seq_t s = sigma_of(n);
    return divisor_count_of((idx_t)s);
}
