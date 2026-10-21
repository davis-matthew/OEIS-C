/*
 * A096015  a(n) = A096014(n) / 2.
 * https://oeis.org/A096015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1, n==1; otherwise * if n is odd → smallest prime divisor
 *            of n * if n is even → smallest prime that does NOT divide n * *
 *            The function uses a pre‑computed list of primes up to 1 000 000;
 *            * for larger candidate primes it falls back on trial division
 *            using the * same list. This implementatio...
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define PRIME_LIMIT 1000000   /* primes up to 1 000 000 */
#define PRIME_CAP   100000    /* enough to hold all primes <= PRIME_LIMIT */

static bool primes_initialized = false;
static unsigned int prime_cnt = 0;
static idx_t primes[PRIME_CAP];

/* Auxiliary routine: initialize the prime list with a simple sieve. */
static void init_prime_list(void)
{
    if (primes_initialized) return;

    static unsigned char is_composite[PRIME_LIMIT + 1];
    /* Mark 0 and 1 as composite */
    is_composite[0] = is_composite[1] = 1;

    for (unsigned int i = 2; i * i <= PRIME_LIMIT; ++i)
    {
        if (!is_composite[i])
        {
            for (unsigned int j = i * i; j <= PRIME_LIMIT; j += i)
                is_composite[j] = 1;
        }
    }

    for (unsigned int i = 2; i <= PRIME_LIMIT && prime_cnt < PRIME_CAP; ++i)
    {
        if (!is_composite[i])
            primes[prime_cnt++] = i;
    }

    primes_initialized = true;
}

/* Trial division primality test using the precomputed prime list. */
static bool is_prime_candidate(idx_t x)
{
    if (x < 2) return false;
    for (unsigned int i = 0; i < prime_cnt && (idx_t)primes[i] * (idx_t)primes[i] <= x; ++i)
        if (x % primes[i] == 0) return false;
    return true;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    init_prime_list();

    if (n & 1) /* odd n: smallest prime factor */
    {
        idx_t m = n;
        for (unsigned int i = 0; i < prime_cnt; ++i)
        {
            unsigned int p = primes[i];
            if ((idx_t)p * (idx_t)p > m) break;
            if (m % p == 0) return (seq_t)p;
        }
        /* n itself is prime */
        return (seq_t)m;
    }
    else /* even n: smallest prime NOT dividing n */
    {
        /* 2 divides n, skip it */
        for (unsigned int i = 1; i < prime_cnt; ++i) /* primes[0] == 2 */
        {
            unsigned int p = primes[i];
            if (n % p != 0) return (seq_t)p;
        }

        /* If all primes in the table divide n (unlikely for n up to typical limits),
           we search for the next missing prime beyond PRIME_LIMIT. */
        idx_t candidate = primes[prime_cnt - 1] + 1;
        if ((candidate & 1) == 0) ++candidate; /* ensure odd */
        while (true)
        {
            if (is_prime_candidate(candidate) && (n % candidate != 0))
                return (seq_t)candidate;
            candidate += 2; /* only odd candidates needed, since 2 divides all evens */
        }
    }
}
