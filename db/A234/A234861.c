/*
 * A234861  Sum of the distinct prime divisors of n^3 - 1.
 * https://oeis.org/A234861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct prime factors of n^3 - 1 (OEIS A234861)
 * Range:     u8 n <= 16, u16 n <= 265, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Internal data for prime sieve (no external headers needed) */
#define SIEVE_LIMIT 20000         /* ≥ sqrt(10000^2 + 10000 + 1) */
#define MAX_PRIMES 2300           /* primes up to 20000 ≈ 2262 */

static bool sieve_ready = false;
static uint64_t primes[MAX_PRIMES];
static size_t prime_cnt = 0;

/* Simple Sieve of Eratosthenes, executed once */
static void init_primes(void)
{
    bool composite[SIEVE_LIMIT + 1] = {false};
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!composite[i]) {
            primes[prime_cnt++] = (uint64_t)i;
            if (prime_cnt == MAX_PRIMES) break;
            for (idx_t j = (idx_t)i * i; j <= SIEVE_LIMIT; j += i) {
                composite[j] = true;
            }
        }
    }
    sieve_ready = true;
}

/* Sum the distinct prime divisors of a positive 64‑bit integer */
static seq_t sum_distinct_primes(uint64_t m)
{
    seq_t sum = 0;
    if (m <= 1) return sum;

    uint64_t value = m;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= value; ++i) {
        uint64_t p = primes[i];
        if (value % p == 0) {
            sum += (seq_t)p;
            while (value % p == 0) value /= p;
        }
    }
    if (value > 1)  /* remaining prime > sqrt(original value) */
        sum += (seq_t)value;
    return sum;
}

/* --------------------------------------------------------------------- */
/* Public sequence function                                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n = 1 ⇒ n^3 - 1 = 0, hence no prime divisors */
    if (n <= 1) return 0;

    if (!sieve_ready) init_primes();

    /* Factor n^3 - 1 = (n - 1)(n^2 + n + 1) */
    uint64_t n_val = (uint64_t)n;
    uint64_t f1 = n_val - 1;                      /* n - 1 */
    uint64_t f2 = n_val * n_val + n_val + 1ULL;   /* n^2 + n + 1 */

    seq_t sum1 = sum_distinct_primes(f1);
    seq_t sum2 = sum_distinct_primes(f2);

    seq_t result = sum1 + sum2;

    /* The only common prime factor of f1 and f2 is 3.  Avoid double counting. */
    if ((f1 % 3ULL == 0) && (f2 % 3ULL == 0))
        result -= (seq_t)3ULL;

    return result;
}
