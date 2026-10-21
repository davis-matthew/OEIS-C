/*
 * A053051  Smallest integer m such that Sum_{k=1..m} d(k) is divisible by n,
 *          where d(k) (A000005) is the number of divisors of k.
 * https://oeis.org/A053051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS definition:
   a(n) = smallest m such that Σ_{k=1..m} d(k) is divisible by n,
   where d(k) is the number of divisors of k (A000005). */

/* Include necessary headers */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Prime sieve parameters */
#define MAX_PRIME_LIMIT 1000000   /* sufficient for divisor factorisation of 64‑bit numbers */
#define PRIMES_SIZE 80000        /* number of primes up to MAX_PRIME_LIMIT */

/* Storage for primes */
static uint32_t primes[PRIMES_SIZE];
static size_t prime_cnt = 0;
static bool primes_inited = false;

/* Simple sieve to populate the prime list */
static void init_primes(void)
{
    if (primes_inited) return;
    static unsigned char is_comp[MAX_PRIME_LIMIT + 1];
    for (uint32_t i = 2; i <= MAX_PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt < PRIMES_SIZE) primes[prime_cnt++] = i;
            /* Avoid overflow when squaring */
            if ((uint64_t)i * i <= MAX_PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    primes_inited = true;
}

/* Compute the divisor count d(n) using the pre‑computed prime list */
static uint64_t divisor_count(uint64_t n)
{
    uint64_t cnt = 1;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        if (tmp % primes[i] == 0) {
            uint64_t exp = 0;
            while (tmp % primes[i] == 0) {
                tmp /= primes[i];
                ++exp;
            }
            cnt *= (exp + 1);
        }
    }
    if (tmp > 1) cnt *= 2;   /* remaining prime factor */
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n == 0 is undefined in the OEIS definition – return 0 */
    if (n == 0) return 0;

    init_primes();

    idx_t sum_mod = 0;
    idx_t m = 1;
    while (true) {
        uint64_t d = divisor_count(m);
        sum_mod = (sum_mod + (idx_t)d) % n;
        if (sum_mod == 0) break;
        ++m;
    }
    return (seq_t)m;
}
