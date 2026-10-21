/*
 * A067776  Numbers k such that binomial(2*k,k) * B(2*k) is an integer where
 *          B(k) are the Bernoulli numbers B(1)=-1/2 B(2)=1/6 B(4)=-1/30...
 *          B(2*m+1)=0 m>1.
 * https://oeis.org/A067776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   p_divides_binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th integer k such that binomial(2*k,k) * B(2*k) is an integer. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/p_divides_binomial.h"

/* Maximum prime for sieve – enough for k up to about 5·10^5 */
#define PRIME_LIMIT 500000
static bool is_prime_sieve[PRIME_LIMIT + 1];
static uint32_t primes[60000];
static int primes_cnt = 0;
static bool sieve_done = false;

static void init_sieve(void) {
    if (sieve_done) return;
    for (int i = 0; i <= PRIME_LIMIT; ++i) is_prime_sieve[i] = true;
    is_prime_sieve[0] = is_prime_sieve[1] = false;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime_sieve[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i) is_prime_sieve[j] = false;
        }
    }
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (is_prime_sieve[i]) primes[primes_cnt++] = (uint32_t)i;
    }
    sieve_done = true;
}

/* simple primality test for numbers > PRIME_LIMIT (works up to about 10^6) */
static bool is_prime_custom(uint64_t n) {
    if (n <= (uint64_t)PRIME_LIMIT) return is_prime_sieve[(int)n];
    if (n < 2) return false;
    for (int i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* check if k satisfies the sequence condition */
static bool good_k(uint64_t k) {
    init_sieve();
    uint64_t N = 2 * k;

    /* factor N */
    uint64_t fp[64];
    int fe[64];
    int fcnt = 0;
    uint64_t tmp = N;
    for (int i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            fp[fcnt] = p;
            fe[fcnt] = e;
            ++fcnt;
        }
    }
    if (tmp > 1) {
        fp[fcnt] = tmp;
        fe[fcnt] = 1;
        ++fcnt;
    }

    /* generate all divisors of N */
    uint64_t divs[2048];
    int dcnt = 1;
    divs[0] = 1;
    for (int i = 0; i < fcnt; ++i) {
        uint64_t p = fp[i];
        int e_max = fe[i];
        int cur_cnt = dcnt;
        for (int j = 0; j < cur_cnt; ++j) {
            uint64_t d = divs[j];
            uint64_t dmul = 1;
            for (int e = 1; e <= e_max; ++e) {
                dmul *= p;
                divs[dcnt++] = d * dmul;
            }
        }
    }

    /* test all primes p such that p-1 divides 2k */
    for (int i = 0; i < dcnt; ++i) {
        uint64_t d = divs[i];
        if (d == 1) continue;                /* corresponds to p = 2, which always divides */
        uint64_t p = d + 1;
        if (!is_prime_custom(p)) continue;    /* skip composite p */
        if (!p_divides_binomial(k, p)) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    init_sieve();
    uint64_t count = 0;
    for (uint64_t k = 1;; ++k) {
        if (good_k(k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
}
