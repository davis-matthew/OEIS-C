/*
 * A108873  Numbers n whose base 3 representations, interpreted as base 10
 *          integers, are semiprimes.
 * https://oeis.org/A108873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the nth integer whose base‑3 representation, read as a
 *            base‑10 integer, is a semiprime
 * Range:     u8 n <= 48, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   base3_to_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/base3_to_decimal.h"

static bool primes_initialized = false;
static const int PRIME_UPPER = 1000000;
static uint32_t primes[80000];
static size_t prime_cnt = 0;

static void init_primes(void) {
    if (primes_initialized) return;
    bool *composite = (bool*)malloc((PRIME_UPPER + 1) * sizeof(bool));
    for (int i = 2; i <= PRIME_UPPER; ++i) {
        if (!composite[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            if ((long long)i * i <= PRIME_UPPER) {
                for (long long j = 1LL * i * i; j <= PRIME_UPPER; j += i)
                    composite[j] = true;
            }
        }
    }
    free(composite);
    primes_initialized = true;
}

static bool is_prime_uint32(uint32_t n) {
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

static bool is_semiprime_uint64(uint64_t y) {
    if (y < 4) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * p;
        if (pp > y) break;
        if (y % p == 0) {
            uint64_t q = y / p;
            if (q <= 0xFFFFFFFFu) {
                return is_prime_uint32((uint32_t)q);
            } else {
                /* q exceeds 32‑bit; use trial division via primes */
                for (size_t j = 0; j < prime_cnt; ++j) {
                    uint32_t qp = primes[j];
                    uint64_t qqp = (uint64_t)qp * qp;
                    if (qqp > q) break;
                    if (q % qp == 0) return false;
                }
                return true;
            }
        }
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    init_primes();
    uint64_t candidate = 1;
    size_t count = 0;
    while (true) {
        uint64_t y = base3_to_decimal(candidate);
        if (is_semiprime_uint64(y)) {
            ++count;
            if (count == (size_t)n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
