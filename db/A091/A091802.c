/*
 * A091802  10^n - (largest n-digit multiple of the n-th prime).
 * https://oeis.org/A091802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = ((10^n - 1) mod p_n) + 1, where p_n is the n-th prime
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest n‑digit multiple of the n‑th prime and subtracts it
   from 10^n. The implementation uses a static sieve to generate primes up
   to a fixed limit once, then calculates the value via modular exponentiation. */

#include <stdbool.h>
#include <stdint.h>

/* -------------------------------------------------------------------------- */
/*  Prime generation (static sieve)                                           */
/* -------------------------------------------------------------------------- */

#define SIEVE_LIMIT   4000000   /* enough for the 10000th prime and beyond   */
#define MAX_PRIMES    300000    /* number of primes stored (≈ 283146 for 4M) */

static uint32_t primes[MAX_PRIMES];
static int   prime_cnt = 0;
static bool  sieve_done = false;

/* Perform the sieve once and fill the primes array. */
static void init_sieve(void)
{
    if (sieve_done) return;

    static bool is_prime[SIEVE_LIMIT + 1];
    int i, j;

    for (i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (i = 2; (int64_t)i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    prime_cnt = 0;
    for (i = 2; i <= SIEVE_LIMIT && prime_cnt < MAX_PRIMES; ++i) {
        if (is_prime[i]) {
            primes[prime_cnt++] = (uint32_t)i;
        }
    }

    sieve_done = true;
}

/* -------------------------------------------------------------------------- */
/*  Main sequence function                                                    */
/* -------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1, return 0           */

    init_sieve();                   /* ensure primes are ready                 */

    if (n > (idx_t)prime_cnt) {
        /* n is larger than precomputed primes; should not happen per spec */
        return 0;
    }

    seq_t p = (seq_t)primes[(int)n - 1];   /* p_n  */

    /* Compute 10^n mod p using modular exponentiation (linear in n).   */
    seq_t pow10_mod = 1;
    for (idx_t i = 0; i < n; ++i) {
        /* pow10_mod and p fit in 64 bits; product < 10 * 4e6 < 4e7     */
        pow10_mod = (pow10_mod * 10ULL) % p;
    }

    /* remainder = (10^n - 1) mod p   */
    seq_t rem = (pow10_mod + p - 1ULL) % p;

    /* a(n) = remainder + 1          */
    return rem + 1ULL;
}
