/*
 * A076882  pi(n) - ceiling(n / log(n)), where pi(n) = the number of primes <=
 *          n.
 * https://oeis.org/A076882
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = pi(n) - ceil(n / log(n))
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where pi(n) is the number of primes <= n. */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

static idx_t cache_max_n = 1;
static int *pi_arr = NULL;

static void
ensure_pi_computed(idx_t n)
{
    if (n <= cache_max_n) return;

    int *new_pi = (int *)realloc(pi_arr, (size_t)(n + 1) * sizeof(int));
    if (!new_pi) abort();
    pi_arr = new_pi;

    char *is_prime = (char *)malloc((size_t)(n + 1));
    if (!is_prime) abort();
    for (size_t i = 0; i <= n; ++i) is_prime[i] = 1;
    if (n >= 0) is_prime[0] = 0;
    if (n >= 1) is_prime[1] = 0;

    size_t limit = (size_t)(sqrt((double)n) + 1.0);
    for (size_t p = 2; p <= limit; ++p)
        if (is_prime[p])
            for (size_t q = p * p; q <= n; q += p)
                is_prime[q] = 0;

    pi_arr[0] = 0;
    pi_arr[1] = 0;
    for (size_t i = 2; i <= n; ++i)
        pi_arr[i] = pi_arr[i - 1] + (int)is_prime[i];

    free(is_prime);
    cache_max_n = n;
}

seq_t
a(idx_t n)
{
    if (n < (idx_t)2) return (seq_t)0;

    ensure_pi_computed(n);
    seq_t pi_n = (seq_t)pi_arr[(size_t)n];

    /* Compute ceil(n / log(n)) without converting a double to seq_t. */
    double nd = (double)n;
    double logn = log(nd);
    double ratio = nd / logn;

    long long base = (long long)ratio;
    long long ceil_val = base + ((double)base < ratio ? 1 : 0);

    return pi_n - (seq_t)ceil_val;
}
