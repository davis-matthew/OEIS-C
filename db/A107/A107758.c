/*
 * A107758  (+2)Sigma(n): If n = Product p_i^r_i then a(n) = Product (2 + Sum
 *          p_i^s_i, s_i=1 to r_i) = Product (1 + (p_i^(r_i+1)-1)/(p_i-1)),
 *          a(1) = 1.
 * https://oeis.org/A107758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^r||n} (2 + ∑_{s=1}^r p^s), where p^r||n means
 *            p^r divides n and p^(r+1) does not. a(1) = 1
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime sieve helpers --------------------------------- */
static uint64_t *prime_list   = NULL;   /* array of primes found so far   */
static size_t   prime_cnt    = 0;       /* number of primes stored        */
static size_t   prime_cap    = 0;       /* allocated size                 */
static uint64_t prime_limit  = 0;       /* largest prime generated so far */

/* Append a new prime to the dynamic array */
static void append_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        uint64_t *tmp = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) return;                /* allocation failure – ignore */
        prime_list = tmp;
        prime_cap  = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

/* Generate all primes up to "limit" inclusively, extending the array
   if necessary. */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_limit) return;

    for (uint64_t cand = (prime_limit < 2 ? 2 : prime_limit + 1); cand <= limit; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if ((unsigned __int128)p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) append_prime(cand);
    }
    prime_limit = limit;
}

/* ---------- Main sequence function -------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t nn   = (uint64_t)n;
    uint64_t tmp  = nn;

    /* Compute ceil(sqrt(nn)) for prime generation limit */
    uint64_t limit = 0;
    while ((unsigned __int128)(limit + 1) * (limit + 1) <= nn) ++limit;
    ensure_primes_upto(limit);

    seq_t product = (seq_t)1;
    size_t idx = 0;

    while (idx < prime_cnt &&
           (unsigned __int128)prime_list[idx] * prime_list[idx] <= tmp) {

        uint64_t p = prime_list[idx];
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }

            /* Compute p^(exp+1) */
            seq_t p_seq = (seq_t)p;
            seq_t pw    = (seq_t)1;
            for (uint64_t k = 0; k < exp + 1; ++k) pw *= p_seq;

            /* term = 1 + (p^(exp+1) - 1)/(p - 1) */
            seq_t term = (seq_t)1 + (pw - (seq_t)1) / (p_seq - (seq_t)1);
            product *= term;
        }
        ++idx;
    }

    if (tmp > 1) {           /* remaining prime factor with exponent 1 */
        seq_t p_seq = (seq_t)tmp;
        seq_t term  = p_seq + (seq_t)2;   /* 1 + (p^2-1)/(p-1) = p+2 */
        product *= term;
    }

    return product;
}
