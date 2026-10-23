/*
 * A138674  a(n) = prime(n)^5 mod prime(n-1).
 * https://oeis.org/A138674
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^5 mod prime(n-1)
 * Range:     u8 n <= 54, u16 n <= 6612, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   modpow5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/modpow5.h"

static seq_t *primes = NULL;          /* 0‑based array of primes */
static idx_t  primes_cnt = 0;         /* number of primes stored   */
static size_t primes_cap = 0;         /* allocated capacity */

/* Grow the primes array to accommodate at least `need` elements. */
static void grow_primes(size_t need)
{
    if (primes_cap >= need)
        return;

    size_t new_cap = primes_cap ? primes_cap * 2 : 128;
    if (new_cap < need)
        new_cap = need;

    seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();            /* memory exhausted */

    primes = tmp;
    primes_cap = new_cap;
}

/* Test whether `n` is prime using the primes already in `primes`. */
static bool is_prime(seq_t n)
{
    if (n < 2)
        return false;

    for (idx_t i = 0; i < primes_cnt; ++i) {
        seq_t p = primes[i];
        if (p * p > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* Ensure that at least `need` primes are in `primes` (1‑based count). */
static void ensure_primes(idx_t need)
{
    if (primes_cnt >= (size_t)need)
        return;              /* already have enough */

    /* Initialize the prime array with the first prime, if empty. */
    if (primes_cnt == 0) {
        grow_primes(128);
        primes[0] = (seq_t)2;
        primes_cnt = 1;
    }

    /* Generate new primes until we have `need` of them. */
    uint64_t cand = (uint64_t)primes[primes_cnt - 1] + 1;
    while (primes_cnt < (size_t)need) {
        if (is_prime((seq_t)cand)) {
            grow_primes(primes_cnt + 1);
            primes[primes_cnt++] = (seq_t)cand;
        }
        ++cand;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;               /* undefined for n < 2; safe placeholder */

    ensure_primes(n);            /* need primes[0..n-1] */

    seq_t p_n   = primes[n - 1];   /* prime(n)   */
    seq_t p_prev = primes[n - 2];  /* prime(n-1) */

    return modpow5(p_n, p_prev);
}
