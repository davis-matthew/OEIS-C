/*
 * A052288  First differences of the average of two consecutive odd primes
 *          (A024675).
 * https://oeis.org/A052288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n+3) - prime(n+1)) / 2
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the nth term of OEIS A052288 (first differences of the average of two consecutive odd primes). */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>   /* for uint32_t */

static idx_t *primes = NULL;   /* 0‑based array of primes */
static size_t primes_len = 0;  /* number of primes stored    */
static size_t primes_cap = 0;  /* allocated capacity         */

/* Grow the primes array until it contains at least 'need' primes (1‑based count). */
static void _ensure_primes_to(idx_t need)
{
    if (primes_len >= (size_t)need)
        return;

    /* Allocate or enlarge the primes array. */
    if (primes_cap == 0) {
        primes_cap = 16;
        primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
    }
    else if (primes_len >= primes_cap) {
        primes_cap <<= 1;
        primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
    }

    /* Find primes by simple trial division, appending until we reach 'need'. */
    idx_t candidate = (primes_len == 0) ? 2 : primes[primes_len - 1] + 1;
    if (candidate <= 3) candidate = 2; /* cover the first prime */

    while (primes_len < (size_t)need) {
        /* Skip even numbers beyond 2. */
        if (candidate == 2 || (candidate & 1U)) {
            bool isprime = true;
            for (size_t i = 0; i < primes_len; ++i) {
                idx_t p = primes[i];
                if (p * p > candidate)
                    break;
                if (candidate % p == 0) {
                    isprime = false;
                    break;
                }
            }
            if (isprime) {
                if (primes_len >= primes_cap) {
                    primes_cap <<= 1;
                    primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
                }
                primes[primes_len++] = candidate;
            }
        }
        ++candidate;
    }
}

/* Return the k-th prime number (1‑based index).  k ≥ 1. */
static idx_t _prime_1based(idx_t k)
{
    if (k == 0)
        return 0;          /* undefined – avoid crash */
    _ensure_primes_to(k);
    return primes[k - 1];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* outside the OEIS offset, behaviour irrelevant */

    idx_t p1 = _prime_1based(n + 1);
    idx_t p2 = _prime_1based(n + 3);
    return ((seq_t)p2 - (seq_t)p1) / (seq_t)2;
}
