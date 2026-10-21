/*
 * A089616  Number of primes between n^2 and (n+1/6)^2.
 * https://oeis.org/A089616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = number of primes p such that n^2 < p ≤ (n + 1/6)^2 (i.e.
 *            p in the interval (n^2 , ((6n+1)/6)^2] ). Implementation uses a
 *            growing prime sieve and a binary search over the prime list
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static state for the prime list and sieve */
static uint32_t *prime_list   = NULL;   /* array of primes */
static size_t   prime_cnt    = 0;       /* number of primes stored */
static size_t   prime_cap    = 0;       /* allocated capacity of prime_list */

static unsigned char *is_composite = NULL;  /* 0 = prime, 1 = composite */
static size_t   comp_size = 0;              /* current size of the sieve */

/* ensure the sieve is extended to include all numbers up to 'limit' */
static void ensure_primes(size_t limit)
{
    if (limit <= comp_size)
        return;

    /* extend the composite array */
    unsigned char *tmp = (unsigned char *)realloc(is_composite, limit + 1);
    if (!tmp) exit(1);
    is_composite = tmp;
    for (size_t i = comp_size + 1; i <= limit; ++i)
        is_composite[i] = 0;

    /* initialise the primes array if needed */
    if (prime_list == NULL) {
        prime_cap = 16;
        prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        if (!prime_list) exit(1);
        prime_cnt = 0;
    }

    /* mark composites using existing primes up to sqrt(limit) */
    size_t start_num = comp_size + 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        if (p2 > limit) break;

        uint64_t start = p2;
        if (start < start_num) {
            start = ((start_num + p - 1) / p) * (uint64_t)p;
        }
        for (uint64_t j = start; j <= limit; j += p)
            is_composite[j] = 1;
    }

    /* collect new primes from (comp_size+1) .. limit */
    for (size_t num = start_num; num <= limit; ++num) {
        if (num < 2) continue;
        if (is_composite[num]) continue;
        if (prime_cnt >= prime_cap) {
            prime_cap *= 2;
            uint32_t *newp = (uint32_t *)realloc(prime_list,
                                                prime_cap * sizeof(uint32_t));
            if (!newp) exit(1);
            prime_list = newp;
        }
        prime_list[prime_cnt++] = (uint32_t)num;
    }

    comp_size = limit;
}

/* number of primes <= value (value fits in uint64) */
static size_t count_primes_le(uint64_t value)
{
    size_t l = 0, r = prime_cnt;
    while (l < r) {
        size_t m = l + (r - l) / 2;
        if ((uint64_t)prime_list[m] <= value)
            l = m + 1;
        else
            r = m;
    }
    return l;   /* count of primes <= value */
}

/* main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* compute the upper bound:
       (n + 1/6)^2 = ((6n + 1)/6)^2 = (6n+1)^2 / 36
       use integer arithmetic and floor division */
    unsigned long long t = 6ULL * (unsigned long long)n + 1ULL;   /* 6n+1   */
    unsigned long long upper = (t * t) / 36ULL;                   /* floor  */
    unsigned long long lower_sq = (unsigned long long)n *
                                (unsigned long long)n;          /* n^2     */

    /* ensure we have primes up to the required upper bound */
    if (upper > comp_size)
        ensure_primes((size_t)upper);

    size_t primes_up_to_upper = count_primes_le(upper);
    size_t primes_up_to_lower = count_primes_le(lower_sq);

    size_t result = 0;
    if (primes_up_to_upper > primes_up_to_lower)
        result = primes_up_to_upper - primes_up_to_lower;

    return (seq_t)result;
}
