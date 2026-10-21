/*
 * A240753  Number of values of k such that n + k + 1 and n + n/k + 1 are both
 *          prime.
 * https://oeis.org/A240753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k | k | n, n + k + 1 and n + n/k + 1 are prime }
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* -------- prime sieve utilities -------- */

static unsigned char *is_prime_arr = NULL;   /* 0 = prime, 1 = composite */
static idx_t prime_limit = 0;

/* Ensure the prime sieve is computed up to at least 'need' (inclusive). */
static void init_prime_cache(idx_t need)
{
    if (prime_limit >= need)
        return;

    /* Allocate a fresh array up to the new limit. */
    idx_t size = need + 1;
    unsigned char *new_arr = (unsigned char *)malloc((size_t)size);
    if (!new_arr)
        return;  /* abort on allocation failure – the caller will fail gracefully */

    for (idx_t i = 0; i < size; ++i)
        new_arr[i] = 0;  /* assume prime */

    new_arr[0] = 1;
    if (size > 1)
        new_arr[1] = 1;

    for (idx_t i = 2; i * i <= need; ++i)
        if (new_arr[i] == 0)
            for (idx_t j = i * i; j <= need; j += i)
                new_arr[j] = 1;

    free(is_prime_arr);
    is_prime_arr = new_arr;
    prime_limit = need;
}

/* -------- recursive divisor enumeration -------- */

static void count_valid_divisors(idx_t n,
                                 const idx_t *factors_p,
                                 const int *factors_e,
                                 int fcnt,
                                 idx_t cur,
                                 seq_t *cnt,
                                 const unsigned char *prime_array)
{
    if (fcnt == 0) {
        idx_t k = cur;
        idx_t v1 = n + k + 1;
        idx_t v2 = n + n / k + 1;
        if (prime_array[v1] == 0 && prime_array[v2] == 0)
            ++(*cnt);
        return;
    }

    int e   = factors_e[fcnt - 1];
    idx_t p = factors_p[fcnt - 1];

    idx_t power = 1;
    for (int i = 0; i <= e; ++i) {
        count_valid_divisors(n, factors_p, factors_e, fcnt - 1,
                             cur * power, cnt, prime_array);
        power *= p;
    }
}

/* -------- the sequence function -------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Ensure we have primes up to 2*n + 1. */
    init_prime_cache(2 * n + 1);

    /* Factor n into its prime powers. */
    idx_t factors_p[20];
    int   factors_e[20];
    int   fcnt = 0;

    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (is_prime_arr[p] == 0 && temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            factors_p[fcnt] = p;
            factors_e[fcnt] = e;
            ++fcnt;
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        factors_p[fcnt] = temp;
        factors_e[fcnt] = 1;
        ++fcnt;
    }

    seq_t count = 0;
    count_valid_divisors(n, factors_p, factors_e, fcnt,
                         1, &count, is_prime_arr);
    return count;
}
