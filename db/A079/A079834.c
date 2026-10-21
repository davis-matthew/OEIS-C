/*
 * A079834  Define f(n)= n + tau(n), then a(n) = k where k is the number of
 *          steps required to get the smallest prime of the form
 *          f(f(f...f(n)))).
 * https://oeis.org/A079834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of applications of f(n)=n+τ(n) needed to reach the
 *            * first prime in the orbit starting at n (τ(n) = divisor
 *            function)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Adjust limit if you know the range of values needed */
#define LIMIT 5000000UL
/* Upper bound for number of primes below LIMIT (prime count ≈ LIMIT/log LIMIT) */
#define PRIME_CAP 400000UL

/* Global tables (static for automatic initialization) */
static uint32_t spf[LIMIT + 1];          /* smallest prime factor */
static uint16_t tau_arr[LIMIT + 1];      /* τ(n) for n ≤ LIMIT */
static uint32_t primes[PRIME_CAP];       /* list of primes up to LIMIT */
static size_t primes_cnt = 0;
static int initialized = 0;

/* -----------------------------------------------------------------------
 *  Sieve and precomputation
 * ----------------------------------------------------------------------- */
static void initTables(void)
{
    if (initialized) return;
    initialized = 1;

    /* 1. Smallest prime factor sieve */
    spf[0] = 0;
    spf[1] = 1;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }

    /* 2. Prime list */
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == (uint32_t)i) {
            primes[primes_cnt++] = (uint32_t)i;
        }
    }

    /* 3. τ(n) table via multiplicativity */
    tau_arr[1] = 1;
    for (size_t i = 2; i <= LIMIT; ++i) {
        uint32_t p = spf[i];
        size_t exp = 0;
        size_t x = i;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }
        tau_arr[i] = tau_arr[x] * (exp + 1);
    }
}

/* -----------------------------------------------------------------------
 *  Helper: τ(n) for n > LIMIT (factorisation using the prime list)
 * ----------------------------------------------------------------------- */
static uint64_t tau64(uint64_t n)
{
    uint64_t result = 1;
    uint64_t rem = n;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = (uint64_t)primes[i];
        if (p * p > rem) break;
        if (rem % p == 0) {
            uint64_t exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            result *= (exp + 1);
        }
    }
    if (rem > 1) /* remaining factor is prime */
        result *= 2;
    return result;
}

/* -----------------------------------------------------------------------
 *  τ(n) accessor (works for both small and large n)
 * ----------------------------------------------------------------------- */
static uint64_t tau_of(uint64_t x)
{
    if (x <= LIMIT) {
        return tau_arr[(size_t)x];
    } else {
        return tau64(x);
    }
}

/* -----------------------------------------------------------------------
 *  Prime test (fast for n ≤ LIMIT, trial division otherwise)
 * ----------------------------------------------------------------------- */
static int isPrime(uint64_t n)
{
    if (n <= LIMIT) {
        return (spf[(size_t)n] == (uint32_t)n) && n > 1;
    }
    if (n < 2) return 0;
    if (n % 2 == 0) return n == 2;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = (uint64_t)primes[i];
        if (p * p > n) break;
        if (n % p == 0) return 0;
    }
    return 1;
}

/* -----------------------------------------------------------------------
 *  Main sequence function
 * ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    initTables();                /* ensure tables are ready */

    uint64_t cur = (uint64_t)n;  /* use 64‑bit for intermediate work */
    uint64_t steps = 0;

    /* Apply f = n + τ(n) until a prime is reached, counting steps. */
    for (;;) {
        cur += tau_of(cur);
        ++steps;
        if (isPrime(cur)) break;
    }

    return (seq_t)steps;
}
