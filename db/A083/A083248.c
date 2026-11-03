/*
 * A083248  Numbers k such that A045763(k) > A000010(k) > A000005(k).
 * https://oeis.org/A083248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9840 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A083248 – numbers k such that A045763(k) > A000010(k) > A000005(k) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper to factor *n* and compute its Euler totient φ(n) and divisor count d(n). */
static void
compute_phi_d(idx_t n, seq_t *phi_res, unsigned int *d_res)
{
    idx_t   m = n;
    seq_t   phi = (seq_t)n;          /* φ starts as n */
    unsigned int d = 1;              /* multiplicative */

    /* factor 2 separately */
    unsigned int exp = 0;
    while ((m & 1) == 0) {
        m >>= 1;
        exp++;
    }
    if (exp > 0) {
        d *= (exp + 1);
        phi = phi / 2;               /* φ * (2-1) / 2 = φ / 2 */
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            d *= (exp + 1);
            phi = phi / (seq_t)p * (seq_t)(p - 1);
        }
    }

    /* remaining prime factor */
    if (m > 1) {
        d *= 2;
        phi = phi / (seq_t)m * (seq_t)(m - 1);
    }

    *phi_res = phi;
    *d_res   = d;
}

/* Storage for the computed sequence terms. */
static seq_t *terms     = NULL;
static size_t term_cnt  = 0;
static size_t term_cap  = 0;

/* Append a term, expanding the backing array if necessary. */
static void
push_term(seq_t val)
{
    if (term_cnt == term_cap) {
        size_t new_cap = term_cap ? term_cap * 2 : 16;
        seq_t *new_arr = mem_resize(seq_t, terms, new_cap);
        if (!new_arr) abort();
        terms = new_arr;
        term_cap = new_cap;
    }
    terms[term_cnt++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n=0, but harmless */

    /* Generate terms until we have at least *n* of them. */
    static idx_t current_k = 1;    /* last integer examined */

    while (term_cnt < (size_t)n) {
        ++current_k;
        seq_t phi;
        unsigned int d;
        compute_phi_d(current_k, &phi, &d);

        seq_t n_seq = (seq_t)current_k;
        seq_t d_seq = (seq_t)d;
        /* unrelateds(k) = k - d(k) - φ(k) + 1, but avoid unsigned underflow */
        seq_t unrelated = 0;
        if (n_seq > d_seq + phi - 1)
            unrelated = n_seq - d_seq - phi + 1;

        if (unrelated > phi && phi > d_seq)
            push_term((seq_t)current_k);
    }

    return terms[(size_t)n - 1];
}
