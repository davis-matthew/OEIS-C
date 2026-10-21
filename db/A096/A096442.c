/*
 * A096442  a(n) = Max { k>0 : denominator(S(k,2n+1)) } where
 *          S(k,s)=sum(i=1,k,i^s*H(i,2)) - H(k,2)*H(k,-s) and
 *          H(k,r)=sum(i=1,k,1/i^r) are the generalized harmonic numbers.
 * https://oeis.org/A096442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator(B₍₂ₙ₎·(n+½)) = 2·denom(B₂ₙ) / gcd(2n+1,
 *            denom(B₂ₙ))
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* Global storage for primes up to the largest limit requested */
static idx_t *prime_array = NULL;
static size_t prime_cnt   = 0;
static idx_t prime_limit = 0;

/* Ensure that primes up to 'limit' are generated */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;                     /* already available */

    /* Allocate temporary array for is_prime flags */
    size_t sz = (size_t)limit + 1;
    bool *is_prime = (bool *)malloc(sz);
    if (!is_prime)
        return;                     /* allocation failure – but should not occur */

    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[(size_t)i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[(size_t)j] = false;

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            ++cnt;

    /* Allocate array for primes */
    idx_t *primes_tmp = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!primes_tmp) {
        free(is_prime);
        return;
    }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            primes_tmp[pos++] = i;

    free(is_prime);
    free(prime_array);            /* discard old array */

    prime_array = primes_tmp;
    prime_cnt   = cnt;
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    /* For n == 0 no meaningful value – return 0 */
    if (n == 0)
        return (seq_t)0;

    idx_t m          = 2 * n;          /* 2n */
    idx_t m_plus_one = m + 1;          /* 2n+1 */
    idx_t limit      = m + 1;          /* primes needed up to 2n+1 */

    ensure_primes_upto(limit);

    seq_t denom = (seq_t)1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_array[i];
        if (p > limit)
            break;
        if (m % (p - 1) == 0)          /* p-1 divides 2n */
            denom *= (seq_t)p;
    }

    seq_t g = gcd(denom, (seq_t)m_plus_one);
    seq_t result = (denom / g) * (seq_t)2;   /* 2*denom(B₂ₙ)/g */

    return result;
}
