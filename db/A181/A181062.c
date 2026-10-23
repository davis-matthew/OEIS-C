/*
 * A181062  Prime powers minus 1.
 * https://oeis.org/A181062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A000961(n) - 1 (prime power minus 1)
 * Range:     u8 n <= 71, u16 n <= 6636, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* A minimal sieve that generates all primes up to LIMIT once.
   The chosen limit is large enough so that the 10 000th prime power
   (and thus every n ≤ 10 000 we are asked to compute) is <= LIMIT.
   Empirically, LIMIT = 300000 gives more than 17000 prime powers
   (primes + squares + cubes + …).  Plenty for n up to 10 000. */
#define PRIME_LIMIT 300000

static bool primes_initialized = false;
static unsigned int primes[PRIME_LIMIT / 2]; /* enough to hold all primes < LIMIT */
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes_initialized)
        return;

    /* Simple boolean sieve */
    bool *is_composite = (bool*)calloc(PRIME_LIMIT + 1, 1);
    if (!is_composite) abort(); /* OOM */

    primes_cnt = 0;
    for (unsigned int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[primes_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (unsigned int j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }

    free(is_composite);
    primes_initialized = true;
}

/* Determine if x is a prime power:
   1 (by definition), any prime, any prime^k (k>1). */
static bool is_prime_power(uint64_t x)
{
    if (x == 1)
        return true;          /* 1 is a prime power by convention */

    /* Find the smallest prime factor */
    uint64_t spf = 0;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= x; ++i) {
        unsigned int p = primes[i];
        if (x % p == 0) {
            spf = p;
            break;
        }
    }
    if (spf == 0)               /* x itself is prime */
        spf = x;

    /* Divide out all occurrences of spf */
    while (x % spf == 0)
        x /= spf;

    return x == 1;
}

/* Return the n-th term of the sequence: a(n) = prime_power(n) - 1 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;             /* undefined for n < 1, but return 0 */

    init_primes();

    uint64_t candidate = 1;
    size_t count = 0;

    while (1) {
        if (is_prime_power(candidate)) {
            ++count;
            if (count == (size_t)n)
                return (seq_t)(candidate - 1);
        }
        ++candidate;
    }
}
