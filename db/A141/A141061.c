/*
 * A141061  Number of primes between n*p(n) and (n+1)*p(n+1), where p(n) is
 *          the n-th prime.
 * https://oeis.org/A141061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p such that n*p(n) < p < (n+1)*p(n+1)
 * Range:     u8 n <= 53, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p(k) is the k-th prime and n >= 1.  The function memoizes
   the required primes so that successive calls are fast. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static uint32_t *gen_primes = NULL;   /* all primes discovered so far */
static size_t gen_count  = 0;         /* number of primes in gen_primes      */
static size_t gen_cap    = 0;         /* allocated capacity of gen_primes    */
static uint32_t gen_next = 2;         /* next candidate to test for primality */

/* Ensure that we have at least 'need' primes in gen_primes. */
static void ensure_n_primes(idx_t need)
{
    if (gen_count >= (size_t)need) return;

    /* Allocate or grow buffer */
    if (gen_cap == 0) {
        gen_cap = 64;
        gen_primes = (uint32_t *)malloc(gen_cap * sizeof(uint32_t));
        if (!gen_primes) abort();
    } else if (gen_count >= gen_cap) {
        gen_cap <<= 1;
        gen_primes = (uint32_t *)realloc(gen_primes, gen_cap * sizeof(uint32_t));
        if (!gen_primes) abort();
    }

    /* Sieve by trial division using already known primes */
    while (gen_count < (size_t)need) {
        uint32_t candidate = gen_next;
        bool is_prime = true;
        uint32_t sqrt_c = (uint32_t)sqrt((double)candidate);

        for (size_t i = 0; i < gen_count && gen_primes[i] <= sqrt_c; ++i) {
            if (candidate % gen_primes[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            gen_primes[gen_count++] = candidate;
        }

        if (candidate == 2)
            gen_next = 3;
        else
            gen_next += 2;  /* skip even numbers */
    }
}

/* Return the n-th prime (1-based).  n >= 1 */
static seq_t get_nth_prime(idx_t n)
{
    if (n == 0) return 0; /* shouldn't happen */
    ensure_n_primes(n);
    return (seq_t)gen_primes[n - 1];
}

/* Count primes in the open interval (low, high) using a segmented sieve.
   low = n * p(n)  (inclusive lower bound)
   high = (n+1)*p(n+1)  (exclusive upper bound)
   Returns only the number of primes strictly between low and high. */
static seq_t count_primes_in_interval(uint64_t low, uint64_t high)
{
    if (low + 1 >= high) return 0;  /* empty interval */

    uint64_t seg_start = low + 1;
    uint64_t seg_end   = high - 1;
    uint64_t seg_len   = seg_end - seg_start + 1;

    /* Create a buffer: 0 = probably prime, 1 = composite */
    char *segment = (char *)calloc(seg_len, 1);
    if (!segment) abort();

    /* For each base prime <= sqrt(high) */
    uint64_t limit = (uint64_t)sqrt((double)high) + 1;
    for (size_t i = 0; i < gen_count && gen_primes[i] <= limit; ++i) {
        uint32_t p = gen_primes[i];
        uint64_t start = (seg_start + p - 1) / p * p;  /* first multiple >= seg_start */
        if (start < (uint64_t)p * p) start = (uint64_t)p * p;
        for (uint64_t x = start; x <= seg_end; x += p) {
            segment[x - seg_start] = 1;
        }
    }

    seq_t cnt = 0;
    for (uint64_t i = 0; i < seg_len; ++i) {
        if (!segment[i]) ++cnt;
    }
    free(segment);
    return cnt;
}

/* External function demanded by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, but return 0 */

    /* Ensure we know the two consecutive primes needed. */
    idx_t idx1 = n;
    idx_t idx2 = n + 1;

    seq_t p1 = get_nth_prime(idx1);  /* p(n)   */
    seq_t p2 = get_nth_prime(idx2);  /* p(n+1) */

    uint64_t low  = (uint64_t)n  * (uint64_t)p1;  /* n * p(n)   */
    uint64_t high = (uint64_t)(n + 1) * (uint64_t)p2; /* (n+1) * p(n+1) */

    return count_primes_in_interval(low, high);
}
