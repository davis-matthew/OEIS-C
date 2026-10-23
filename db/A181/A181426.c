/*
 * A181426  Numerator of Sum_{k=1..n} k^4 / Product_{k=1..n} k^4.
 * https://oeis.org/A181426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = Numerator of (∑_{k=1..n} k⁴) / (∏_{k=1..n} k⁴)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 144, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         = (∑_{k=1..n} k⁴) / gcd(∑_{k=1..n} k⁴, (n!)⁴) */
/* This implementation computes the sum S = n(n+1)(2n+1)(3n²+3n−1)/30
   and divides out the common prime powers with (n!)⁴ using prime
   factorisation up to n.  It works for any seq_t and idx_t defined
   elsewhere. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define PRIME_LIMIT 11000          /* > expected maximum n (≈10000) */
#define MAX_PRIMES 2000            /* enough for primes ≤ PRIME_LIMIT */

/* Prime sieve (static, initialised once). */
static uint32_t primes[MAX_PRIMES];
static size_t   prime_cnt = 0;
static bool     primes_ready = false;
static uint8_t  sieve_flag[PRIME_LIMIT + 1] = {0};

static void init_primes(void)
{
    if (primes_ready)
        return;

    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!sieve_flag[i]) {
            if (prime_cnt < MAX_PRIMES)
                primes[prime_cnt++] = (uint32_t)i;
            if (i * i <= PRIME_LIMIT) {
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    sieve_flag[j] = 1;
            }
        }
    }
    primes_ready = true;
}

seq_t a(idx_t n)
{
    init_primes();                     /* Ensure primes are ready */

    /* Compute the sum S = n(n+1)(2n+1)(3n²+3n−1)/30. */
    seq_t nseq = (seq_t)n;
    seq_t n2   = nseq * nseq;
    seq_t term1 = nseq * (nseq + 1);
    seq_t term2 = (seq_t)2 * nseq + 1;
    seq_t term3 = (seq_t)3 * n2 + (seq_t)3 * nseq - 1;
    seq_t S = term1 * term2 * term3;   /* intermediate product */
    S /= (seq_t)30;                    /* exact division */

    seq_t result = S;                  /* will be reduced by gcd */

    /* For each prime p ≤ n, remove common powers from result. */
    for (size_t idx = 0; idx < prime_cnt && primes[idx] <= (uint32_t)n; ++idx) {
        uint32_t p = primes[idx];

        /* Exponent of p in S (eS). */
        idx_t eS = 0;
        seq_t tmpS = S;
        while (tmpS % (seq_t)p == 0) {
            tmpS /= (seq_t)p;
            ++eS;
        }

        /* Exponent of p in n! (Legendre's formula). */
        idx_t eFact = 0;
        idx_t div = n;
        while (div > 0) {
            div /= p;
            eFact += div;
        }
        eFact *= 4;                    /* exponent in (n!)⁴ */

        /* Common exponent to remove. */
        idx_t exp = (eS < eFact) ? eS : eFact;
        if (exp == 0)
            continue;

        /* Divide result by p^exp. */
        seq_t power = 1;
        for (idx_t k = 0; k < exp; ++k)
            power *= (seq_t)p;
        result /= power;
    }

    return result;
}
