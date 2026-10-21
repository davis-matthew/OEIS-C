/*
 * A085051  Smallest multiple of n sandwiched between two numbers both having
 *          square divisors.
 * https://oeis.org/A085051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest multiple of n that is sandwiched between two
 *            numbers, each having a non‑trivial square divisor
 * Range:     u8 n <= 3, u16 n <= 89, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For a given integer m, this function returns true iff m-1 and m+1 both contain
   a square factor.  It searches for square factors by trial division using
   a precomputed list of primes up to 1,000,000.  Zero is treated as having
   an infinite square divisor, which satisfies the requirement for the
   lower bound. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* --------- Prime sieve (up to 1,000,000) --------- */
#define PRIME_MAX 1000000
static bool prime_sieved = false;
static uint32_t primes[78498];          /* number of primes <= 1e6 */
static size_t prime_count = 0;

static void init_primes(void)
{
    if (prime_sieved) return;

    static bool is_prime[PRIME_MAX + 1];
    for (size_t i = 2; i <= PRIME_MAX; ++i) is_prime[i] = true;

    for (size_t i = 2; i * i <= PRIME_MAX; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= PRIME_MAX; j += i)
                is_prime[j] = false;

    for (size_t i = 2; i <= PRIME_MAX; ++i)
        if (is_prime[i]) primes[prime_count++] = (uint32_t)i;

    prime_sieved = true;
}

/* ---------- Check for square divisor ---------- */
static bool has_square_factor(seq_t x)
{
    if (x == 0) return true;          /* 0 is divisible by every square */
    if (x < 4) return false;          /* 1,2,3 cannot have a square factor */

    init_primes();

    /* Check all primes up to sqrt(x) that we have */
    for (size_t pi = 0; pi < prime_count; ++pi) {
        seq_t p = (seq_t)primes[pi];
        seq_t p2 = p * p;
        if (p2 > x) break;
        if ((x % p2) == 0) return true;
    }

    /* If sqrt(x) exceeds our prime list, continue with odd numbers
       beyond PRIME_MAX.  This should rarely happen for the ranges we
       handle (n <= 10,000). */
    seq_t start = (prime_count > 0 ? (seq_t)primes[prime_count - 1] + 2 : 2);
    for (seq_t p = start; p * p <= x; p += 2) {
        seq_t p2 = p * p;
        if ((x % p2) == 0) return true;
    }

    return false;
}

/* ---------------- Sequence function ---------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; n=0 not used */

    seq_t k = 1;
    for (;;) {
        seq_t m = (seq_t)n * k;
        if (has_square_factor(m - 1) && has_square_factor(m + 1))
            return m;
        ++k;
    }
}
