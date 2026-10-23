/*
 * A178045  Numbers of semiprimes between consecutive even numbers that are
 *          not semiprimes.
 * https://oeis.org/A178045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of semiprimes x with 2*A141468(n) < x <
 *            2*A141468(n+1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* array of nonprime numbers (0 = 0, 1 = 1, 2 = 4, …) */
static idx_t *nonprimes   = NULL;
static size_t np_len      = 0, np_cap      = 0;
static idx_t  current_value = 0;   /* last number tested for primality */

/* array of primes, used for semiprime test */
static idx_t *primes      = NULL;
static size_t primes_len  = 0, primes_cap   = 0;
static idx_t  primes_max  = 1;     /* largest number already tested for primes */

/* generic dynamic array growth (capacity is stored in *cap) */
static void ensure_capacity(void **buf, size_t */*len*/, size_t *cap, size_t needed,
                            size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = *cap ? *cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    void *tmp = realloc(*buf, new_cap * elem_size);
    if (!tmp) abort();
    *buf   = tmp;
    *cap   = new_cap;
}

/* generate primes up to 'limit' (inclusive) */
static void ensure_primes_up_to(idx_t limit)
{
    if (primes_max >= limit) return;
    for (idx_t num = primes_max + 1; num <= limit; ++num) {
        if (is_prime(num)) {
            ensure_capacity((void **)&primes, &primes_len, &primes_cap,
                            primes_len + 1, sizeof(idx_t));
            primes[primes_len++] = num;
        }
    }
    primes_max = limit;
}

/* test whether n is a semiprime */
static bool is_semiprime(idx_t n)
{
    if (n < 4) return false;
    idx_t x = n;
    unsigned int factor_cnt = 0;
    for (size_t i = 0; i < primes_len; ++i) {
        idx_t p = primes[i];
        if (p > x / p) break;
        while (x % p == 0) {
            ++factor_cnt;
            if (factor_cnt > 2) return false;
            x /= p;
        }
    }
    if (x > 1) ++factor_cnt;
    return factor_cnt == 2;
}

/* ensure that we have at least 'desired_len' nonprimes in the array */
static void ensure_nonprimes_up_to_len(size_t desired_len)
{
    if (np_len >= desired_len) return;

    /* First element must be 0 */
    if (np_len == 0) {
        ensure_capacity((void **)&nonprimes, &np_len, &np_cap, 1, sizeof(idx_t));
        nonprimes[0]    = 0;
        np_len          = 1;
        current_value   = 0;
    }

    while (np_len < desired_len) {
        ++current_value;
        if (!is_prime(current_value)) {
            ensure_capacity((void **)&nonprimes, &np_len, &np_cap,
                            np_len + 1, sizeof(idx_t));
            nonprimes[np_len++] = current_value;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* we need nonprimes up to indices n and n+1 (0‑based positions n-1 and n) */
    ensure_nonprimes_up_to_len((size_t)(n + 1));

    idx_t L = (idx_t)((seq_t)nonprimes[(size_t)(n - 1)] * 2);
    idx_t R = (idx_t)((seq_t)nonprimes[(size_t)n] * 2);

    /* make sure we have primes up to the right bound for the semiprime test */
    ensure_primes_up_to(R);

    seq_t count = 0;
    if (R > L + 1) {
        for (idx_t x = L + 1; x < R; ++x) {
            if (is_semiprime(x)) ++count;
        }
    }
    return count;
}
