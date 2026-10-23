/*
 * A024532  a(n) = [ (3rd elementary symmetric function of P(n))/(first
 *          elementary symmetric function of P(n)) ], where P(n) = {1, p(1),
 *          p(2), ..., p(n-1)}, where p(0) = 1.
 * https://oeis.org/A024532
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (e2 + e3) / e1 ) where P(n) = {1, p(1), …, p(n‑1)}
 *            e1 – sum of the elements of P(n) e2 – sum of all pairwise
 *            products of the primes in P(n) e3 – sum of all triple products
 *            of the primes in P(n) a(n) = [ e3 / e1 ] (integer floor) e2 is
 *            updated by S2 += (sum_of_primes_so_far) * p e3 i...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

/* static state for prime generation and prefix sums */
static uint32_t *prime_arr = NULL;
static size_t   prime_cnt = 0;
static size_t   prime_cap = 0;

static seq_t  *e1_arr = NULL;
static seq_t  *S2_arr = NULL;
static seq_t  *S3_arr = NULL;
static size_t  summax  = 0;   /* highest k for which e1[summax]… are computed */

/* build at least k primes, extending the array if needed */
static void ensure_primes(size_t k)
{
    if (prime_cnt >= k) return;

    /* approximate upper bound for the k‑th prime (for k≥6) */
    size_t limit;
    if (k < 6) {
        limit = 15;                     /* covers the first 5 primes */
    } else {
        double dk = (double)k;
        limit = (size_t)(dk * (log(dk) + log(log(dk))) + 3.0);
    }

    while (1) {
        /* sieve up to limit */
        uint8_t *is_comp = (uint8_t*)malloc(limit + 1);
        if (!is_comp) { abort(); }
        for (size_t i = 0; i <= limit; ++i) is_comp[i] = 0;

        for (size_t i = 2; i * i <= limit; ++i) {
            if (!is_comp[i]) {
                for (size_t j = i * i; j <= limit; j += i) is_comp[j] = 1;
            }
        }

        /* count primes up to limit */
        size_t cnt = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (!is_comp[i]) ++cnt;

        if (cnt >= k) {                   /* we have enough */
            /* extend prime array if required */
            if (prime_cap < cnt) {
                prime_arr = (uint32_t*)realloc(prime_arr, cnt * sizeof(uint32_t));
                prime_cap = cnt;
            }

            size_t idx = 0;
            for (size_t i = 2; i <= limit && idx < cnt; ++i)
                if (!is_comp[i]) prime_arr[idx++] = (uint32_t)i;

            prime_cnt = cnt;
            free(is_comp);
            return;
        }

        /* not enough primes – increase limit and retry */
        free(is_comp);
        limit = limit * 2 + 10;
    }
}

/* recompute the prefix sums up to new_max_k if needed */
static void ensure_sums(size_t new_max_k)
{
    if (summax >= new_max_k) return;

    /* enlarge arrays */
    size_t new_cap = new_max_k + 1;
    if (e1_arr == NULL || e1_arr == (seq_t*)0x1) { /* first allocation */
        e1_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
        S2_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
        S3_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!e1_arr || !S2_arr || !S3_arr) { abort(); }
        /* initial state: k = 0 (only element 1) */
        e1_arr[0] = (seq_t)1;
        S2_arr[0] = (seq_t)0;
        S3_arr[0] = (seq_t)0;
    } else {
        /* extend existing arrays */
        size_t old_cap = summax + 1;
        e1_arr = (seq_t*)realloc(e1_arr, new_cap * sizeof(seq_t));
        S2_arr = (seq_t*)realloc(S2_arr, new_cap * sizeof(seq_t));
        S3_arr = (seq_t*)realloc(S3_arr, new_cap * sizeof(seq_t));
        if (!e1_arr || !S2_arr || !S3_arr) { abort(); }
    }

    /* compute new entries incrementally */
    for (size_t j = summax + 1; j <= new_max_k; ++j) {
        seq_t p  = (seq_t)prime_arr[j - 1];        /* prime with index j-1 */
        seq_t e1_prev = e1_arr[j - 1];
        seq_t S2_prev = S2_arr[j - 1];
        seq_t S3_prev = S3_arr[j - 1];
        seq_t sum_primes = e1_prev - (seq_t)1;     /* sum of previous primes */
        e1_arr[j] = e1_prev + p;
        S2_arr[j] = S2_prev + sum_primes * p;
        S3_arr[j] = S3_prev + S2_prev * p;
    }
    summax = new_max_k;
}

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;     /* offset is 3; behaviour below does not matter */

    size_t k = (size_t)(n - 1);      /* number of primes needed */

    ensure_primes(k);
    ensure_sums(k);

    seq_t e1 = e1_arr[k];
    seq_t e2 = S2_arr[k];
    seq_t e3 = S3_arr[k];
    seq_t e3_total = e2 + e3;
    return e3_total / e1;           /* integer floor division */
}
