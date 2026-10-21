/*
 * A073882  Number of primes between n and n^2.
 * https://oeis.org/A073882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(n^2) – pi(n-1) (number of primes p with n ≤ p ≤ n²)
 * Range:     u8 n <= 41, u16 n <= 907, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* static state for the prime sieve */
static uint32_t *prime_list  = NULL;   /* array of primes */
static size_t  prime_cnt   = 0;        /* number of primes in prime_list */
static idx_t   sieve_limit = 0;        /* maximum value that has been sieved */

/* forward declarations */
static void build_sieve(idx_t limit);
static size_t count_primes_upto(idx_t x);

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset starts at 1, but handle 0 safely */
        return 0;

    /* compute n² in 128‑bit to avoid overflow */
    __uint128_t n2_128 = ( __uint128_t )n * n;
    if (n2_128 > ( __uint128_t )UINT64_MAX)  /* cannot handle larger limits */
        return 0;

    idx_t n2 = (idx_t)n2_128;

    /* enlarge sieve if necessary */
    if (n2 > sieve_limit)
        build_sieve(n2);

    size_t cnt_n2 = count_primes_upto(n2);          /* π(n²)          */
    size_t cnt_n1 = (n > 1) ? count_primes_upto(n-1) : 0;  /* π(n‑1)  */

    return (seq_t)(cnt_n2 - cnt_n1);
}

/* ---------------------------------------------------------------------- */
/* Build a sieve of Eratosthenes up to 'limit' (inclusive) and store the
   primes in prime_list.  Only odd numbers are stored in the bitset to
   reduce memory consumption. */
static void build_sieve(idx_t limit)
{
    /* free old list */
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }

    if (limit < 2) {                    /* nothing to sieve */
        sieve_limit = limit;
        return;
    }

    /* allocate bitset for odd numbers: index i corresponds to number 2*i+1 */
    size_t odd_count = (limit >> 1) + 1;            /* include 1 */
    size_t bit_bytes = (odd_count >> 3) + 1;
    unsigned char *bits = (unsigned char *)calloc(bit_bytes, 1);
    if (!bits) { /* allocation failed, nothing to do */ return; }

    idx_t sqrt_limit = (idx_t) sqrt((double)limit);
    for (idx_t i = 3; i <= sqrt_limit; i += 2) {
        size_t idx = i >> 1;
        if (!(bits[idx >> 3] & (1U << (idx & 7)))) { /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i << 1) {
                size_t jdx = j >> 1;
                bits[jdx >> 3] |= (1U << (jdx & 7));
            }
        }
    }

    /* estimate number of primes to allocate the list */
    double dlimit = (double)limit;
    size_t est_cnt = (size_t)(dlimit / (log(dlimit) - 1.0)) + 10;
    prime_list = (uint32_t *)malloc(est_cnt * sizeof(uint32_t));
    if (!prime_list) { free(bits); return; }

    prime_cnt = 0;
    /* add 2 if within range */
    if (limit >= 2) {
        prime_list[prime_cnt++] = 2;
    }

    for (idx_t num = 3; num <= limit; num += 2) {
        size_t idx_n = num >> 1;
        if (!(bits[idx_n >> 3] & (1U << (idx_n & 7)))) {  /* prime */
            if (prime_cnt >= est_cnt) {  /* resize if needed */
                est_cnt = est_cnt + (est_cnt >> 1) + 10;
                uint32_t *tmp = (uint32_t *)realloc(prime_list, est_cnt * sizeof(uint32_t));
                if (!tmp) { /* allocation failed, abort */
                    free(bits);
                    free(prime_list);
                    prime_list = NULL;
                    prime_cnt = 0;
                    return;
                }
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = (uint32_t)num;
        }
    }

    free(bits);
    sieve_limit = limit;
}

/* ---------------------------------------------------------------------- */
/* Counts how many primes in prime_list are ≤ x.  Binary search on the
   sorted prime list.  All primes are ≤ sieve_limit. */
static size_t count_primes_upto(idx_t x)
{
    if (prime_list == NULL || x < 2)
        return 0;

    size_t lo = 0;
    size_t hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= (uint32_t)x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;            /* number of primes ≤ x */
}
