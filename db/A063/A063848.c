/*
 * A063848  Numbers k such that mu(k) + mu(k+1) + mu(k+2) = 2.
 * https://oeis.org/A063848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   numbers k such that μ(k)+μ(k+1)+μ(k+2)=2 (OEIS A063848). This
 *            implementation builds a sieve of Möbius values up to a growing
 *            limit and stores the qualifying k’s in a dynamic array. The
 *            results are cached so that each value is computed only once,
 *            regardless of call order
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* dynamic arrays and caches */
static seq_t *k_list   = NULL;   /* list of qualifying k's   */
static size_t k_len   = 0;       /* number of elements stored */
static size_t k_cap   = 0;       /* current buffer capacity   */
static size_t max_n   = 0;       /* largest n requested so far */

/* helper to ensure k_list has enough capacity */
static void ensure_k_cap(size_t needed)
{
    if (needed <= k_cap) return;
    size_t new_cap = k_cap ? k_cap : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(k_list, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    k_list = tmp;
    k_cap = new_cap;
}

/* build Möbius array up to 'limit' and collect qualifying k's */
static void build_sequence(idx_t limit)
{
    /* allocate Möbius array */
    int8_t *mu = (int8_t *)calloc(limit + 1, sizeof(int8_t));
    if (!mu) abort();           /* out of memory */
    bool *comp = (bool *)calloc(limit + 1, sizeof(bool));
    if (!comp) abort();

    /* linear sieve for Möbius */
    mu[1] = 1;
    int32_t *pr = NULL;  /* prime list */
    size_t primes = 0;
    size_t prime_cap = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            /* i is prime */
            if (primes == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                int32_t *tmp = (int32_t *)realloc(pr, prime_cap * sizeof(int32_t));
                if (!tmp) abort();
                pr = tmp;
            }
            pr[primes++] = (int32_t)i;
            mu[i] = -1;             /* one prime factor -> odd -> -1 */
        }
        for (size_t j = 0; j < primes; ++j) {
            idx_t p = pr[j];
            idx_t v = i * p;
            if (v > limit) break;
            comp[v] = true;
            if (i % p == 0) {
                mu[v] = 0;
                break;
            } else {
                mu[v] = -mu[i];
            }
        }
    }

    /* collect qualifying k's */
    ensure_k_cap(limit / 2 + 10);  /* a rough upper bound */
    k_len = 0;
    for (idx_t k = 1; k + 2 <= limit; ++k) {
        if (mu[k] + mu[k + 1] + mu[k + 2] == 2) {
            if (k_len == k_cap) ensure_k_cap(k_cap * 2);
            k_list[k_len++] = (seq_t)k;
        }
    }

    /* cleanup */
    free(mu);
    free(comp);
    free(pr);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined, but guard */
    if (n > max_n) {
        /* need more terms; estimate a generous limit */
        idx_t limit = n * 20;
        if (limit < 200000) limit = 200000;
        while (true) {
            build_sequence(limit);
            if (k_len >= (size_t)n) break;
            limit *= 2;            /* increase and retry */
        }
        max_n = n;
    }
    return k_list[n - 1];
}
