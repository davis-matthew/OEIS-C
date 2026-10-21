/*
 * A069789  Smallest k>n such that sigma(n) divides sigma(k).
 * https://oeis.org/A069789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k > n such that sigma(n) divides sigma(k)
 * Range:     u8 n <= 63, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* ------------------------------------------------------------ */
/*   Prime sieve utilities (used only for factorising numbers)  */
/* ------------------------------------------------------------ */
static uint32_t *prime_list   = NULL;   /* list of primes up to prime_limit */
static size_t   prime_cnt    = 0;        /* number of primes stored          */
static size_t   prime_limit  = 0;        /* maximum value included in prime_list */

/*  Extend prime_list so that it contains all primes ≤ max_limit   */
static void prime_sieve_up_to(size_t max_limit)
{
    if (max_limit <= prime_limit) return;        /* already good            */

    /* Allocate array, initialise all entries to 1 (prime)            */
    unsigned char *is_prime = (unsigned char*)calloc(max_limit + 1, 1);
    if (!is_prime) abort();

    if (max_limit >= 0) is_prime[0] = 0;
    if (max_limit >= 1) is_prime[1] = 0;
    for (size_t i = 2; i <= max_limit; ++i) is_prime[i] = 1;

    size_t sq = (size_t)sqrt((double)max_limit);
    for (size_t i = 2; i <= sq; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= max_limit; j += i)
                is_prime[j] = 0;

    size_t cnt = 0;
    for (size_t i = 2; i <= max_limit; ++i)
        if (is_prime[i]) ++cnt;

    uint32_t *new_list = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!new_list) abort();

    size_t idx = 0;
    for (size_t i = 2; i <= max_limit; ++i)
        if (is_prime[i]) new_list[idx++] = (uint32_t)i;

    free(prime_list);
    prime_list = new_list;
    prime_cnt  = cnt;
    prime_limit = max_limit;
}

/* Helper: integer square root (floor), works for idx_t up to 2^63-1 */
static inline idx_t isqrt_idx(idx_t n)
{
    return (idx_t)sqrt((double)n);
}

/* Sum of divisors of n (n ≥ 1).  Works for any seq_t size.  */
static seq_t sigma_of(idx_t n)
{
    if (n <= 1) return 1;             /* sigma(1) = 1 */

    seq_t result = 1;
    idx_t tmp = n;
    idx_t limit = isqrt_idx(tmp);
    if (limit < 2) limit = 2;
    prime_sieve_up_to((size_t)limit);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if (p > limit) break;
        if (tmp % p == 0) {
            seq_t term = 1;
            seq_t power = 1;
            while (tmp % p == 0) {
                tmp /= p;
                power *= (seq_t)p;
                term  += power;
            }
            result *= term;
            limit = isqrt_idx(tmp);
            if (limit < 2) limit = 2;
        }
    }
    if (tmp > 1) {               /* remaining prime factor */
        result *= (1 + (seq_t)tmp);
    }
    return result;
}

/* ------------------------------------------------------------ */
/*                       Main sequence function                 */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    seq_t sig_n = sigma_of(n);

    for (idx_t k = n + 1;; ++k) {
        seq_t sig_k = sigma_of(k);
        if ((sig_k % sig_n) == 0)
            return k;
    }
}
