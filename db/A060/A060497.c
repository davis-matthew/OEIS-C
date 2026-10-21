/*
 * A060497  a(n) = binomial(2^n, prime(n)).
 * https://oeis.org/A060497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(2^n , prime(n))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   binomial_big, pow_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n is 1‑based (a(1) = 1).  The function works for all n ≥ 1.
   It lazily builds a small prime sieve (up to 200 000) and
   computes the binomial multiplicatively so that the result
   fits whatever implementation of seq_t is provided. */

/* ---------- include standard headers ----------
   The build supplies the types seq_t and idx_t.  ---------------------------------*/
#include <stdbool.h>
#include <stddef.h>
#include "../utils/binomial_big.h"
#include "../utils/pow_two.h"

/* ---------- small prime sieve (200 000) ----------
   We keep the sieve array inside the init function to avoid
   static VLA warnings in some C++ compilers.  ---------------------------------*/
#define PRIME_LIMIT 200000   /* > the 10 000th prime (104729) */

static int  primes[19000];   /* enough to hold all primes <= PRIME_LIMIT   */
static size_t primes_cnt = 0;
static bool   prime_sieve_initialized = false;

static void
init_prime_sieve(void)
{
    if (prime_sieve_initialized)
        return;

    /* temporary local sieve array */
    static bool is_prime[PRIME_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t p = 2; p * p <= PRIME_LIMIT; ++p)
        if (is_prime[p])
            for (size_t k = p * p; k <= PRIME_LIMIT; k += p)
                is_prime[k] = false;

    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            primes[primes_cnt++] = (int)i;

    prime_sieve_initialized = true;
}

/* ---------- lookup of the n‑th prime (1‑based) ----------
   We assume n is in [1, primes_cnt].  ---------------------------------*/
static int
nth_prime(idx_t n)
{
    init_prime_sieve();
    return primes[(size_t)n - 1];
}

/* ---------- the required sequence function ----------
   It works for any n ≥ 1, in any call order.  */
seq_t
a(idx_t n)
{
    /* guard against n < 1 – behaviour is undefined, but we
       simply return 0 for safety. */
    if (n < 1)
        return (seq_t)0;

    int p = nth_prime(n);          /* p_n, the n‑th prime          */
    seq_t N = pow_two(n);          /* N = 2^n, as seq_t            */
    return binomial_big(N, (idx_t)p);
}
