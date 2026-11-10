/*
 * A057759  Least nonnegative square root of 3 mod n for n in A057125.
 * https://oeis.org/A057759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least x >= 0 with x^2 == 3 (mod m), where m = A057125(n);
 *            found by search over x
 * Range:     u8 n <= 133, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 80 terms through n = 888 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define A057759_PRIME_LIMIT 100000

static uint64_t A057759_primes[A057759_PRIME_LIMIT];
static size_t A057759_prime_cnt = 0;
static bool A057759_primes_init = false;

static void A057759_init_primes(void)
{
    if (A057759_primes_init) return;
    bool *is_comp = (bool*)calloc(A057759_PRIME_LIMIT + 1, sizeof(bool));
    for (size_t i = 2; i <= A057759_PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            A057759_primes[A057759_prime_cnt++] = i;
            if (i * i <= A057759_PRIME_LIMIT) {
                for (size_t j = i * i; j <= A057759_PRIME_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    A057759_primes_init = true;
}

static bool A057759_passes(uint64_t m)
{
    if ((m % 4) == 0 || (m % 9) == 0) return false;
    uint64_t x = m;
    for (size_t i = 0; i < A057759_prime_cnt; ++i) {
        uint64_t p = A057759_primes[i];
        if (p * p > x) break;
        if (x % p == 0) {
            if (p == 2 || p == 3) {
                while (x % p == 0) x /= p;
                continue;
            }
            if (p % 12 != 1 && p % 12 != 11) return false;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) {
        if (x == 2 || x == 3) return true;
        if (x % 12 != 1 && x % 12 != 11) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    A057759_init_primes();

    idx_t count = 0;
    uint64_t m = 1;
    while (true) {
        if (A057759_passes(m)) {
            ++count;
            if (count == n) break;
        }
        ++m;
    }

    uint64_t target = 3 % m;
    for (uint64_t x = 0; x < m; ++x) {
        if ((x * x) % m == target) return (seq_t)x;
    }
    return (seq_t)0;
}
