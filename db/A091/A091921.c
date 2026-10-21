/*
 * A091921  Sum of odd proper distinct prime divisors of n. That is, the sum
 *          of odd distinct prime divisors of n that are less than n.
 * https://oeis.org/A091921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = sum of odd proper distinct prime divisors of n
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: factorise n, sum its distinct prime factors, subtract 2 if n is even, */
/* and return 0 for odd primes (including 1). */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* static storage of primes for trial division */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;
static uint32_t prime_max  = 0;

/* append a new prime to the list, growing the array if necessary */
static void add_prime(uint32_t p)
{
    if (prime_count == prime_cap)
    {
        size_t new_cap = prime_cap ? prime_cap << 1 : 16;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
        if (tmp == NULL) /* allocation failure – leave list unchanged */
            return;
        prime_list = tmp;
        prime_cap = new_cap;
    }
    prime_list[prime_count++] = p;
    if (p > prime_max) prime_max = p;
}

/* test primality of a candidate using the current prime list */
static int is_prime_candidate(uint32_t cand)
{
    uint32_t sq = (uint32_t)sqrt((double)cand);
    for (size_t i = 0; i < prime_count; ++i)
    {
        uint32_t p = prime_list[i];
        if (p > sq) break;
        if (cand % p == 0) return 0;
    }
    return 1;
}

/* extend the prime list up to and including 'limit' */
static void ensure_primes_upto(uint32_t limit)
{
    if (prime_max >= limit) return;
    uint32_t start = prime_max ? prime_max + 1 : 2;
    for (uint32_t cand = start; cand <= limit; ++cand)
        if (is_prime_candidate(cand))
            add_prime(cand);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;  /* a(1) = 0, a(0) irrelevant */

    /* generate primes up to sqrt(n) */
    uint32_t lim = (uint32_t)floor(sqrt((double)n));
    ensure_primes_upto(lim);

    idx_t temp = n;
    seq_t sum = 0;       /* sum of distinct prime factors of n */

    for (size_t i = 0; i < prime_count; ++i)
    {
        uint32_t p = prime_list[i];
        uint64_t p2 = (uint64_t)p * p;
        if (p2 > temp) break;
        if (temp % p == 0)
        {
            sum += (seq_t)p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) /* remaining prime factor > sqrt(original) */
        sum += (seq_t)temp;

    /* adjust according to parity and primality */
    if ((n & 1) == 0) /* even n – exclude the factor 2 */
        return sum - 2;
    else if (sum == n) /* n is an odd prime (or n==1 handled earlier) */
        return 0;
    else
        return sum; /* odd composite */
}
