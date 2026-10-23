/*
 * A108636  Semiprimes with even digits.
 * https://oeis.org/A108636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = 2*A036956(n) (semiprimes whose decimal digits are all
 *            even)
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   all_even_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/all_even_digits.h"

/* ---------- prime sieve ---------- */
static bool primes_initialized = false;
static uint32_t *primes = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;

/* generate a list of primes up to LIMIT (enough for n <= 10000) */
static void init_primes(void)
{
    if (primes_initialized) return;

    const size_t LIMIT = 300000;            /* covers >15624 even‑digit numbers */
    bool *is_prime = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;

    /* count primes */
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++prime_cnt;

    primes = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) primes[idx++] = (uint32_t)i;

    free(is_prime);
    primes_initialized = true;
}

static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4) return false;
    uint64_t n = x;
    int factors = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        while ((n % p) == 0) {
            ++factors;
            n /= p;
            if (factors > 2) return false;
        }
    }
    if (n > 1) ++factors;                 /* remaining prime factor */
    return factors == 2;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; behaviour for 0 is not used */

    init_primes();

    uint64_t count = 0;
    uint64_t num   = 4;                     /* first semiprime >= 4 */

    while (true) {
        uint64_t last = num % 10;
        if (last == 2 || last == 4 || last == 6 || last == 8) {
            if (all_even_digits(num) && is_semiprime_uint64(num)) {
                ++count;
                if (count == n) return (seq_t)num;
            }
        }
        ++num;
    }
}
