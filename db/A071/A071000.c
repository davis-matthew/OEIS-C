/*
 * A071000  Numbers m such that the denominator of Sum_{k=1..m} 1/gcd(m,k)
 *          equals m.
 * https://oeis.org/A071000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 190, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A071000 – numbers m such that the denominator of
   Σ_{k=1..m} 1/gcd(m,k) equals m.
   Using Σ_{k=1..m} 1/gcd(m,k) = (1/m)·Σ_{d|m} d·φ(d),
   the reduced denominator is m exactly when gcd(Σ d·φ(d), m)=1. */

/* standard headers */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>

/* compute φ(d) once up to a given limit */
static unsigned long long *phi_table = NULL;   /* φ values stored as 64‑bit */
static size_t phi_cap = 0;                     /* capacity of phi_table */

/* extend   φ   table to cover all numbers up to `limit` (inclusive) */
static void ensure_phi_limit(size_t limit)
{
    if (limit <= phi_cap)
        return;

    size_t new_cap = limit + 1;               /* we want indices 0..limit */
    if (new_cap < 2 * phi_cap)                /* grow at most twice */
        new_cap = 2 * phi_cap;
    if (new_cap == 0)                         /* first time */
        new_cap = 256;

    unsigned long long *new_tbl = (unsigned long long*)realloc(phi_table,
                                          new_cap * sizeof(unsigned long long));
    if (!new_tbl)
        abort();                               /* out of memory */

    phi_table = new_tbl;
    phi_cap   = new_cap;

    /* compute φ for all indices 1..phi_cap-1 */
    for (size_t i = 0; i < phi_cap; ++i)
        phi_table[i] = i;                     /* default φ(n)=n */
    if (phi_cap > 0)
        phi_table[0] = 0;
    if (phi_cap > 1)
        phi_table[1] = 1;

    for (size_t p = 2; p < phi_cap; ++p) {
        if (phi_table[p] == p) {              /* p is prime */
            for (size_t j = p; j < phi_cap; j += p)
                phi_table[j] -= phi_table[j] / p;
        }
    }
}

/* Euclidean algorithm on 64‑bit unsigned integers */
static unsigned long long gcd64(unsigned long long a, unsigned long long b)
{
    while (b) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* cache of sequence values */
static seq_t   *terms   = NULL;          /* terms[0] is a(1) */
static size_t  terms_len = 0;            /* number of cached terms */
static size_t  terms_cap = 0;            /* capacity of terms array */

/* ensure the cache has room for at least `min` elements */
static void ensure_terms_capacity(void)
{
    if (terms_len < terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 1024;
    seq_t *new_arr = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_arr)
        abort();
    terms = new_arr;
    terms_cap = new_cap;
}

/* add a new term to the cache */
static void add_term(seq_t val)
{
    ensure_terms_capacity();
    terms[terms_len++] = val;
}

/* global state: next integer to be examined */
static idx_t next_m = 1;

/* generate the first `target` terms of the sequence */
static void generate_terms(size_t target)
{
    while (terms_len < target) {
        idx_t m = next_m++;

        /* ensure φ table covers m */
        ensure_phi_limit((size_t)m);

        /* Compute T(m) = Σ_{d|m} d·φ(d)  using 64‑bit arithmetic. */
        unsigned long long T = 0;
        for (size_t d = 1; d * d <= (size_t)m; ++d) {
            if (m % d == 0) {
                size_t d2 = m / d;
                T += (unsigned long long)d * phi_table[d];
                if (d2 != d)
                    T += (unsigned long long)d2 * phi_table[d2];
            }
        }

        /* Check gcd(T, m) == 1  */
        if (gcd64(T, (unsigned long long)m) == 1)
            add_term((seq_t)m);
    }
}

/* external interface required by the framework */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n = 0 */

    generate_terms((size_t)n);
    return terms[(size_t)n - 1];
}
