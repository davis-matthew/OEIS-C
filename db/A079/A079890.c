/*
 * A079890  Least number > n having one more prime factor than n, not
 *          necessarily distinct.
 * https://oeis.org/A079890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = least m > n with A001222(m) = A001222(n)+1 (A001222 =
 *            big–omega: number of prime factors counted with multiplicity)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *spf = NULL;          /* smallest prime factor */
static uint8_t *omega = NULL;         /* big–omega values */
static uint32_t *primes = NULL;       /* list of primes for linear sieve */
static size_t primes_cnt = 0;
static size_t primes_cap = 0;
static size_t max_limit = 2000000;    /* adjust if larger n is needed */
static bool initialized = false;

static void init_sieve(void)
{
    size_t i, p, limit = max_limit;
    spf = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    omega = (uint8_t*)calloc(limit + 1, sizeof(uint8_t));
    primes_cap = 200000;
    primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    omega[1] = 0;
    for (i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            omega[i] = 1;
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (p = 0; p < primes_cnt; ++p) {
            uint32_t pp = primes[p];
            uint64_t prod = (uint64_t)i * pp;
            if (pp > spf[i] || prod > limit) break;
            spf[(size_t)prod] = pp;
            omega[(size_t)prod] = omega[i] + 1;
        }
    }
}

static inline uint8_t get_omega(uint32_t n)
{
    return omega[n];
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_sieve();
        initialized = true;
    }
    uint32_t start = (uint32_t)n + 1;
    if (start <= 1) start = 2;
    uint8_t target = get_omega((uint32_t)n) + 1;
    for (uint32_t m = start; m <= max_limit; ++m) {
        if (get_omega(m) == target) {
            return (seq_t)m;
        }
    }
    /* If not found within pre‑computed range, extend the sieve on demand */
    size_t cur_limit = max_limit;
    uint32_t m = max_limit + 1;
    while (true) {
        /* Expand the sieve by a factor of 2 */
        size_t new_limit = cur_limit * 2;
        if (new_limit < cur_limit) new_limit = (size_t)UINT32_MAX; /* overflow guard */
        spf = (uint32_t*)realloc(spf, new_limit + 1);
        omega = (uint8_t*)realloc(omega, new_limit + 1);
        memset(spf + cur_limit + 1, 0, (new_limit - cur_limit) * sizeof(uint32_t));
        memset(omega + cur_limit + 1, 0, (new_limit - cur_limit) * sizeof(uint8_t));

        for (uint32_t i = cur_limit + 1; i <= new_limit; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                omega[i] = 1;
                if (primes_cnt == primes_cap) {
                    primes_cap <<= 1;
                    primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
                }
                primes[primes_cnt++] = i;
            }
            for (size_t p = 0; p < primes_cnt; ++p) {
                uint32_t pp = primes[p];
                uint64_t prod = (uint64_t)i * pp;
                if (pp > spf[i] || prod > new_limit) break;
                spf[(size_t)prod] = pp;
                omega[(size_t)prod] = omega[i] + 1;
            }
        }
        max_limit = new_limit;
        cur_limit = new_limit;
        /* continue searching from current position */
        for (; m <= new_limit; ++m) {
            if (get_omega(m) == target) {
                return (seq_t)m;
            }
        }
    }
}
