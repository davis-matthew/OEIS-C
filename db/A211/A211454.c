/*
 * A211454  (p-1)/x, where p = prime(n) and x = ord(9,p), the smallest
 *          positive integer such that 9^x == 1 mod p.
 * https://oeis.org/A211454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)-1)/ord(9,prime(n))
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the multiplicative order of 9 modulo the n-th prime. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"

static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* Ensure that prime_list contains at least 'needed' primes. */
static void ensure_primes(idx_t needed)
{
    if (prime_count >= (size_t)needed)
        return;

    /* Estimate a limit large enough to contain 'needed' primes. */
    idx_t limit = prime_limit ? prime_limit * 2 : (needed * 20 + 100);
    if (limit < 100)
        limit = 100;

    uint64_t *new_primes = NULL;
    size_t new_count = 0;
    build_primes_upto(limit, &new_primes, &new_count);

    if (prime_list)
        free(prime_list);          /* free any previous allocation */
    prime_list   = new_primes;
    prime_count  = new_count;
    prime_limit  = limit;

    /* If we are still short, double the limit again. */
    if (prime_count < (size_t)needed)
        ensure_primes(needed);
}

seq_t a(idx_t n)
{
    if (n == 0)          /* safe guard: OEIS offset is 1 */
        return 0;

    ensure_primes(n);
    seq_t p = (seq_t)prime_list[n-1];          /* n-th prime (1‑based) */

    if (p == 3)          /* 9 ≡ 0 (mod 3), no multiplicative order */
        return 0;

    seq_t order = p - 1;      /* order divides p-1 */
    uint64_t temp = (uint64_t)(p - 1);

    /* Factorise p-1 using the prime list (which is now large enough). */
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] *
                                   (uint64_t)prime_list[i] <= temp; ++i)
    {
        uint64_t q = prime_list[i];
        if (temp % q != 0)
            continue;

        while (temp % q == 0)
            temp /= q;        /* remove all copies of q */

        /* Reduce the candidate order by this prime factor. */
        while (order % (seq_t)q == 0)
        {
            seq_t cand = order / (seq_t)q;
            if (mod_pow((seq_t)9, (uint64_t)cand, p) == 1)
                order = cand;
            else
                break;
        }
    }

    /* If a large prime factor remains. */
    if (temp > 1)
    {
        uint64_t q = temp;
        while (order % (seq_t)q == 0)
        {
            seq_t cand = order / (seq_t)q;
            if (mod_pow((seq_t)9, (uint64_t)cand, p) == 1)
                order = cand;
            else
                break;
        }
    }

    return (p - 1) / order;
}
