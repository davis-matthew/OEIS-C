/*
 * A196053  The first Zagreb index of the rooted tree with Matula-Goebel
 *          number n.
 * https://oeis.org/A196053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; if n = prime(t) then a(n) = a(t) + 2 + 2*G(t); if n =
 *            r*s (r,s >= 2) then a(n) = a(r) + a(s) - G(r)^2 - G(s)^2 +
 *            G(n)^2; G(m) = number of prime factors of m counted with
 *            multiplicity (bigomega)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static idx_t cap = 0;                     /* current allocated size (≥ n+1) */
static seq_t *a_val = NULL;               /* stored a(n) values           */
static bool   *a_done = NULL;             /* flags for computed values    */
static idx_t  *spf = NULL;                /* smallest prime factor        */
static idx_t  *prime_idx = NULL;          /* 0 if composite, else 1‑based
                                              index of prime in ascending order */
static idx_t   prime_count = 0;            /* number of primes seen so far */

/* Compute the number of prime factors with multiplicity for n (uses spf). */
static idx_t bigomega(idx_t n)
{
    idx_t cnt = 0;
    while (n > 1) {
        cnt++;
        n = n / spf[n];
    }
    return cnt;
}

/* Extend the cached arrays to accommodate indices up to new_cap-1. */
static void expand(idx_t new_cap)
{
    if (new_cap <= cap) return;

    /* Reallocate memory for arrays, preserving existing data. */
    seq_t  *new_val       = (seq_t *)realloc(a_val, new_cap * sizeof(seq_t));
    bool   *new_done      = (bool *)realloc(a_done, new_cap * sizeof(bool));
    idx_t  *new_spf       = (idx_t *)realloc(spf, new_cap * sizeof(idx_t));
    idx_t  *new_prime_idx = (idx_t *)realloc(prime_idx, new_cap * sizeof(idx_t));
    if (!new_val || !new_done || !new_spf || !new_prime_idx) abort();

    /* Initialize freshly allocated slots. */
    for (idx_t i = cap; i < new_cap; ++i) {
        new_done[i] = false;
        new_spf[i]  = 0;
        new_prime_idx[i] = 0;
    }

    /* Update the global state. */
    a_val       = new_val;
    a_done      = new_done;
    spf         = new_spf;
    prime_idx   = new_prime_idx;
    cap         = new_cap;

    /* Compute spf and prime indices for numbers that were not computed before. */
    for (idx_t i = 2; i < cap; ++i) {
        if (spf[i] != 0) continue;          /* already processed in a previous expansion */

        idx_t f = 0;
        for (idx_t j = 2; j * j <= i; ++j) {
            if (i % j == 0) { f = j; break; }
        }
        if (!f) {            /* i is prime */
            f = i;
            ++prime_count;
            prime_idx[i] = prime_count;
        }
        spf[i] = f;
    }
}

/* Ensure that the cache can answer queries for n (including n itself). */
static void ensure_cache(idx_t n)
{
    expand(n + 1);                     /* indices up to n inclusive */

    /* Initialise the base case if it has not been done yet. */
    if (!a_done[1]) {
        a_done[1] = true;
        a_val[1]  = 0;
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;            /* safety for the base case */

    ensure_cache(n);
    if (a_done[n]) return a_val[n];

    seq_t result;
    if (prime_idx[n] != 0) {         /* n is prime.  t = prime_idx[n] */
        idx_t t = prime_idx[n];
        seq_t a_t = a(t);
        seq_t G_t = (seq_t)bigomega(t);
        result = a_t + (seq_t)2 + (seq_t)2 * G_t;
    } else {                         /* n is composite: k = r * s, r = spf[n] */
        idx_t r = spf[n];
        idx_t s = n / r;

        seq_t a_r = a(r);
        seq_t a_s = a(s);
        seq_t G_r = (seq_t)bigomega(r);
        seq_t G_s = (seq_t)bigomega(s);
        seq_t G_n = (seq_t)bigomega(n);

        result = a_r + a_s
               - G_r * G_r
               - G_s * G_s
               + G_n * G_n;
    }

    a_val[n]  = result;
    a_done[n] = true;
    return result;
}
