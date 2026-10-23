/*
 * A100533  3^(p^2) where p = prime(n).
 * https://oeis.org/A100533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(prime(n)^2), where prime(n) is the n-th prime, offset
 *            = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   pow_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_base3.h"

/* --------------------------------------------------------------------- */
/* Prime sieve ---------------------------------------------------------- */
/* --------------------------------------------------------------------- */
#define SIEVE_LIMIT 200000   /* enough to contain at least the 10000th prime */

static bool sieve_done = false;
static uint32_t primes[SIEVE_LIMIT];
static int prime_count = 0;

/* Build a simple sieve of Eratosthenes filling the global primes array. */
static void build_sieve(void)
{
    if (sieve_done) {
        return;
    }

    /* local sieve array – compile‑time constant size */
    static bool is_comp[SIEVE_LIMIT + 1] = {0};
    int i, j, cnt = 0;

    is_comp[0] = is_comp[1] = true;
    for (i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[cnt++] = (uint32_t)i;
            if ((int64_t)i * i <= SIEVE_LIMIT) {
                for (j = i * i; j <= SIEVE_LIMIT; j += i) {
                    is_comp[j] = true;
                }
            }
        }
    }
    prime_count = cnt;
    sieve_done = true;
}

/* Return the n-th prime (1‑indexed).  n must be >= 1 and <= prime_count. */
static uint32_t nth_prime(idx_t n)
{
    build_sieve();
    return primes[(uint32_t)n - 1];
}

/* --------------------------------------------------------------------- */
/* Exponentiation ------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Sequence function ----------------------------------------------------- */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) {      /* offset starts at 1 – behaviour for n==0 is irrelevant */
        return 0;
    }

    uint32_t p = nth_prime(n);
    seq_t exp = (seq_t)p * (seq_t)p;
    return pow_base3(exp);
}
