/*
 * A077573  Smallest number of the form (10^k -1)/9 == 0 (mod prime(n)). with
 *          a(1) = a(3) = 0.
 * https://oeis.org/A077573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 6, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   build_primes_upto, get_prime, mod_pow, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A077573: a(n) = smallest repunit (10^k – 1)/9 that is 0 (mod prime(n)),
   with a(1) = a(3) = 0.
   For primes p > 5 the minimal k is the multiplicative order of 10
   modulo p; for p = 3 the minimal k is 3; for p = 2 or 5 no repunit
   is divisible by p, so a(n)=0.
*/
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
/* prime(n)  */
#include "../utils/get_prime.h"
/* small prime list for factoring p-1  */
#include "../utils/build_primes_upto.h"
/* modular exponentiation modulo a seq_t  */
#include "../utils/mod_pow.h"
/* integer exponentiation with correct sequence type  */
#include "../utils/seq_pow.h"

/* helpers for caching a small list of primes up to a limit */
static uint64_t *small_prime_list = NULL;
static size_t small_prime_cnt   = 0;
static const idx_t SMALL_PRIME_LIMIT = 400;  /* > sqrt(104729) */

/* initialise the small prime list once */
static void init_small_primes(void)
{
    if (small_prime_list != NULL)
        return;
    build_primes_upto(SMALL_PRIME_LIMIT, &small_prime_list, &small_prime_cnt);
}

/* compute the multiplicative order of 10 modulo the odd prime p (>5) */
static idx_t multiplicative_order(idx_t p)
{
    /* factor p-1 only by the small primes */
    idx_t m = p - 1;
    idx_t order = m;
    idx_t temp = m;
    for (size_t i = 0; i < small_prime_cnt && (uint64_t)small_prime_list[i] * small_prime_list[i] <= (uint64_t)temp; ++i) {
        idx_t q = small_prime_list[i];
        if (temp % q == 0) {
            /* remove all occurrences of q from temp */
            while (temp % q == 0)
                temp /= q;
            /* try to reduce the order using q */
            while (order % q == 0) {
                idx_t candidate = order / q;
                if (mod_pow((seq_t)10, candidate, (seq_t)p) == 1ULL)
                    order = candidate;
                else
                    break;
            }
        }
    }
    /* remaining factor > sqrt(p-1) */
    if (temp > 1) {
        idx_t q = temp;
        while (order % q == 0) {
            idx_t candidate = order / q;
            if (mod_pow((seq_t)10, candidate, (seq_t)p) == 1ULL)
                order = candidate;
            else
                break;
        }
    }
    return order;
}

seq_t a(idx_t n)
{
    /* get the n-th prime (1‑based index) */
    idx_t p = get_prime(n);

    /* primes 2 and 5 have no terminating repunit */
    if (p == 2 || p == 5)
        return (seq_t)0;

    /* prime 3: the first divisible repunit is 111 (k = 3) */
    if (p == 3)
        return (seq_t)111;

    /* ensure we have the small prime table for factoring p-1 */
    init_small_primes();

    /* compute k = multiplicative order of 10 modulo p */
    idx_t k = multiplicative_order(p);

    /* compute repunit R_k = (10^k – 1)/9 */
    seq_t pow10k = seq_pow((seq_t)10, k);
    seq_t repunit = pow10k - 1;
    repunit /= 9;

    return repunit;
}
