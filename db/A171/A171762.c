/*
 * A171762  a(n) = Sum_{k=n^2+1..(n+1)^2-1} tau(k).
 * https://oeis.org/A171762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=n^2+1 .. (n+1)^2-1} tau(k)
 * Range:     u8 n <= 18, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Pre‑computed primes up to 10001 – sufficient for n ≤ 10 000. */
#define PRIME_LIMIT 10001
#define PRIME_COUNT 1229

static uint32_t primes[PRIME_COUNT];
static size_t prime_cnt = 0;
static bool primes_initialized = false;

/* Simple sieve of Eratosthenes – called once. */
static void init_primes(void)
{
    if (primes_initialized) return;
    bool sieve[PRIME_LIMIT + 1] = {false};
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            primes[prime_cnt++] = i;
            if (i <= PRIME_LIMIT / i) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    sieve[j] = true;
            }
        }
    }
    primes_initialized = true;
}

/* Number of divisors of a 64‑bit integer k.  Works for all
   seq_t types that support +, *, etc. */
static seq_t tau(uint64_t k)
{
    seq_t res = 1;
    uint64_t tmp = k;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        if (tmp % primes[i] == 0) {
            uint32_t exp = 0;
            do {
                tmp /= primes[i];
                ++exp;
            } while (tmp % primes[i] == 0);
            res *= (exp + 1);
        }
    }
    if (tmp > 1)  /* remaining prime factor */
        res *= 2;
    return res;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 0, but harmless */

    init_primes();

    /* Bounds for the summation – all fit in 64 bits for n ≤ 10 000. */
    uint64_t n_u      = (uint64_t)n;
    uint64_t start    = n_u * n_u + 1;
    uint64_t end      = (n_u + 1) * (n_u + 1) - 1;

    seq_t sum = 0;
    for (uint64_t k = start; k <= end; ++k)
        sum += tau(k);

    return sum;
}
