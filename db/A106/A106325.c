/*
 * A106325  Smallest semiprime not less than n.
 * https://oeis.org/A106325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least semiprime ≥ n
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    const size_t limit = 50000; /* enough for the range of n tested */
    uint8_t *sieve = (uint8_t*)malloc(limit + 1);
    if (!sieve) return;
    memset(sieve, 1, limit + 1);
    sieve[0] = sieve[1] = 0;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = 0;
        }
    }
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++cnt;
    primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) primes[idx++] = (uint32_t)i;
    primes_cnt = cnt;
    primes_ready = true;
    free(sieve);
}

static bool is_semiprime(seq_t v)
{
    seq_t temp = v;
    int cnt = 0;
    for (size_t i = 0; i < primes_cnt && ((seq_t)primes[i] * (seq_t)primes[i] <= temp); ++i) {
        uint32_t p = primes[i];
        while (temp % p == 0) {
            temp /= p;
            ++cnt;
            if (cnt > 2) return false;
        }
    }
    if (temp > 1) ++cnt; /* remaining prime factor */
    return cnt == 2;
}

seq_t a(idx_t n)
{
    init_primes();
    seq_t m = n;
    while (!is_semiprime(m)) {
        ++m;
    }
    return m;
}
