/*
 * A069248  Number of positive divisors of n themselves divisible by largest
 *          prime that divides n.
 * https://oeis.org/A069248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n)*E_n/(E_n+1) (d(n) = number of divisors of n, E_n =
 *            exponent of the largest prime divisor of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_PRIME_SIEVE 1000000   /* primes up to 1e6 */
#define MAX_PRIMES      80000     /* sufficient for primes up to 1e6 */

static uint32_t prime_list[MAX_PRIMES];
static size_t   prime_count = 0;
static bool     primes_generated = false;

/* Simple sieve: generate all primes up to MAX_PRIME_SIEVE once. */
static void generate_primes(void)
{
    if (primes_generated)
        return;

    static unsigned char sieve[MAX_PRIME_SIEVE + 1];
    for (int i = 0; i <= MAX_PRIME_SIEVE; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (int i = 2; i * i <= MAX_PRIME_SIEVE; ++i)
        if (sieve[i]) {
            for (int j = i * i; j <= MAX_PRIME_SIEVE; j += i)
                sieve[j] = 0;
        }

    for (int i = 2; i <= MAX_PRIME_SIEVE && prime_count < MAX_PRIMES; ++i)
        if (sieve[i])
            prime_list[prime_count++] = (uint32_t)i;

    primes_generated = true;
}

seq_t a(idx_t n_idx)
{
    /* the sequence is defined for n >= 1; argument n_idx is of type idx_t (uint64_t) */
    uint64_t n = (uint64_t)n_idx;
    if (n == 1)
        return (seq_t)0;   /* a(1) = 0 */

    generate_primes();

    uint64_t remaining = n;
    uint64_t exp_of_largest = 0;

    seq_t divisors = (seq_t)1;    /* starts with 1, then multiply by (ei+1) for each prime factor */

    /* factor n using primes up to sqrt(remaining) */
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= remaining; ++i) {
        uint64_t p = prime_list[i];
        if (remaining % p == 0) {
            int exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++exp;
            }
            /* the last processed prime is the largest when iterating in ascending order */
            exp_of_largest = (uint64_t)exp;
            divisors *= (seq_t)(exp + 1);  /* multiply by (ei+1) */
        }
    }

    /* if a prime factor > sqrt(original n) remains, its exponent is 1 */
    if (remaining > 1) {
        exp_of_largest = 1;
        divisors *= (seq_t)2;   /* (1 + 1) = 2 */
    }

    /* a(n) = d(n) * E / (E + 1) */
    seq_t result = divisors / ((seq_t)exp_of_largest + 1);
    result *= (seq_t)exp_of_largest;

    return result;
}
