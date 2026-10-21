/*
 * A078287  Least nontrivial multiple of the n-th prime beginning with 3.
 * https://oeis.org/A078287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1583, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A078287:
   a(n) = the least multiple of the n-th prime p_n (p_n * m, m ≥ 2)
          whose decimal representation begins with the digit 3 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_PRIME_LIMIT 200000        /* enough for >10 000 primes */
#define MAX_PRIME_COUNT 200000

/* Storage for primes */
static uint32_t primes[MAX_PRIME_COUNT];
static size_t prime_count = 0;
static bool initialized = false;

/* Simple sieve of Eratosthenes up to MAX_PRIME_LIMIT */
static void init_primes(void)
{
    if (initialized)
        return;

    bool is_prime[MAX_PRIME_LIMIT + 1];
    for (size_t i = 0; i <= MAX_PRIME_LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= MAX_PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    prime_count = 0;
    for (size_t i = 2; i <= MAX_PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            primes[prime_count++] = (uint32_t)i;
            if (prime_count >= MAX_PRIME_COUNT)
                break;
        }
    }
    initialized = true;
}

/* Return the leading decimal digit of a 64‑bit integer x (x ≥ 1) */
static uint32_t leading_digit(uint64_t x)
{
    while (x >= 10ULL)
        x /= 10ULL;
    return (uint32_t)x;
}

/* Public function required by the test harness   */
seq_t a(idx_t n)
{
    /* a(1) corresponds to the first term in the OEIS listing. */
    if (n == 0)
        return (seq_t)0;     /* undefined entry – return 0 */

    init_primes();

    /* ensure n is within the pre‑computed range */
    if ((size_t)n > prime_count)
        return (seq_t)0;     /* safety fallback – not expected for n ≤ 10 000 */

    uint32_t p = primes[(size_t)(n - 1)];   /* nth prime, 1‑based */

    /* search for the smallest multiple with first decimal digit 3 */
    for (uint64_t m = 2ULL;; ++m) {
        uint64_t val = (uint64_t)p * m;
        if (leading_digit(val) == 3U)
            return (seq_t)val;
    }
}
