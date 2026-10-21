/*
 * A207291  Polya-Vinogradov numbers A177865 for primes p == 1 (mod 4).
 * https://oeis.org/A207291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   compute_pv_for_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = max_{0<k<p_n} | sum_{i=1..k} (i/p_n) |,
 *           where p_n is the n‑th prime ≡ 1 (mod 4) and (i/p) is the Legendre symbol.
 *           In other words, a(n) is the Polya‑Vinogradov number for primes ≡1 mod 4.
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/compute_pv_for_prime.h"

/*---------------------------------------------------------------*/
/*  Global storage for primes                                          */
/*---------------------------------------------------------------*/
static uint64_t *prime_list   = NULL;   /* array of primes */
static size_t   prime_count   = 0;      /* number of primes stored */

/*---------------------------------------------------------------*/
/*  Sieve to generate all primes up to 'limit'.                     */
/*  The primes are stored in the global array prime_list.           */
/*---------------------------------------------------------------*/
static void sieve_primes_upto(idx_t limit)
{
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }

    /* Allocate marker array (C++ requires explicit cast) */
    unsigned char *is_prime = (unsigned char*)malloc((size_t)(limit + 1));
    if (!is_prime)
        return;
    memset(is_prime, 1, (size_t)(limit + 1));
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; (i * i) <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    /* Store primes */
    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) {
        free(is_prime);
        return;
    }
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            prime_list[idx++] = (uint64_t)i;

    prime_count = cnt;
    free(is_prime);
}

/*---------------------------------------------------------------*/
/*  Ensure that we have at least 'n' primes ≡1 (mod 4).             */
/*  Sieve limits are doubled until enough primes are found.        */
/*---------------------------------------------------------------*/
static void ensure_prime1mod4_for_n(idx_t n)
{
    static idx_t current_limit = 2000;   /* starting limit */

    for (;;) {
        sieve_primes_upto(current_limit);

        idx_t count1 = 0;
        for (size_t i = 0; i < prime_count; ++i) {
            if (prime_list[i] % 4ULL == 1ULL) {
                ++count1;
                if (count1 == n)
                    return;          /* enough primes found */
            }
        }
        current_limit *= 2;              /* increase sieve range */
    }
}

/*---------------------------------------------------------------*/
/*  Return the n-th prime ≡1 (mod 4).  Must have been pre‑computed.*/
/*---------------------------------------------------------------*/
static uint64_t nth_prime1mod4(idx_t n)
{
    idx_t count = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        if (prime_list[i] % 4ULL == 1ULL) {
            ++count;
            if (count == n)
                return prime_list[i];
        }
    }
    return 0;   /* should not happen for valid input */
}

/*---------------------------------------------------------------*/
/*  Compute the Polya‑Vinogradov number for a prime p.            */

/*---------------------------------------------------------------*/
/*  Main routine exposed to the framework.                        */
/*---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n=0; return 0 for safety */
        return 0;

    /* Ensure we have enough primes ≡1 (mod 4). */
    ensure_prime1mod4_for_n(n);

    /* Retrieve the n‑th prime and compute its Polya‑Vinogradov number. */
    uint64_t p = nth_prime1mod4(n);
    if (p == 0)
        return 0;       /* safety failure */

    return compute_pv_for_prime(p);
}
