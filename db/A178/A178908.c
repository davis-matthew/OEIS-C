/*
 * A178908  GF(2) sum of divisors of n.
 * https://oeis.org/A178908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the sum of divisors of the degree‑n GF(2)[X]
 *            polynomial whose coefficients are the bits of n
 * Range:     u8 n <= 255, u16 n <= 2047, u32 n <= 2047, u64 n <= 2047,
 *            u128 n <= 2047, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"   /* bitlen_seq_t(seq_t) */

static inline int poly_degree(seq_t x)
{
    return (int)bitlen_seq_t(x) - 1;
}

/* XOR polynomial multiplication (over GF(2)) */
static inline seq_t poly_mul(seq_t a, seq_t b)
{
    seq_t r = 0;
    while (b) {
        if (b & 1) r ^= a;
        a <<= 1;
        b >>= 1;
    }
    return r;
}

/* Remainder of a divided by b (b != 0) */
static inline seq_t poly_rem(seq_t a, seq_t b)
{
    int db = poly_degree(b);
    while (a && poly_degree(a) >= db) {
        int shift = poly_degree(a) - db;
        a ^= b << shift;
    }
    return a;
}

/* Exact division factor (b divides a) */
static inline seq_t poly_div_exact(seq_t a, seq_t b)
{
    seq_t q = 0;
    int db = poly_degree(b);
    while (a && poly_degree(a) >= db) {
        int shift = poly_degree(a) - db;
        q ^= ((seq_t)1) << shift;
        a ^= b << shift;
    }
    return q;
}

/* Table of irreducible monic polynomials */
static seq_t *irreds      = NULL;
static size_t irred_cnt   = 0;
static size_t irred_cap   = 0;
static int    irred_maxdeg = 0;

/* Generate all irreducible monic polynomials up to degree maxdeg */
static void ensure_irreds_up_to_deg(int maxdeg)
{
    if (irred_maxdeg >= maxdeg) return;

    for (int d = irred_maxdeg + 1; d <= maxdeg; ++d) {
        seq_t base = ((seq_t)1) << d;          /* monic term x^d */
        uint64_t total = (uint64_t)1 << d;      /* number of d‑bit masks */

        for (uint64_t mask = 0; mask < total; ++mask) {
            seq_t cand = base | (seq_t)mask;
            bool is_irred = true;
            for (size_t i = 0; i < irred_cnt; ++i) {
                seq_t q = irreds[i];
                if (poly_degree(q) > d / 2) break;
                if (poly_rem(cand, q) == 0) { is_irred = false; break; }
            }
            if (is_irred) {
                if (irred_cnt >= irred_cap) {
                    irred_cap = irred_cap ? irred_cap * 2 : 32;
                    irreds = (seq_t *)realloc(irreds, irred_cap * sizeof(seq_t));
                }
                irreds[irred_cnt++] = cand;
            }
        }
    }
    irred_maxdeg = maxdeg;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, n=0 unused */

    seq_t m = (seq_t)n;              /* polynomial represented by n */
    int   deg = poly_degree(m);

    ensure_irreds_up_to_deg(deg);

    seq_t result = 1;                /* multiplicative identity */

    for (size_t i = 0; i < irred_cnt; ++i) {
        seq_t p = irreds[i];
        int   pd = poly_degree(p);
        if (pd > deg) break;

        int exp = 0;
        while (poly_rem(m, p) == 0) {
            m = poly_div_exact(m, p);
            ++exp;
        }
        if (exp > 0) {
            seq_t term = 1;   /* p^0 */
            seq_t S = 1;      /* sum so far */
            for (int k = 1; k <= exp; ++k) {
                term = poly_mul(term, p);
                S ^= term;
            }
            result = poly_mul(result, S);
        }
    }

    if (m != 1) {              /* remaining factor is irreducible with exp=1 */
        seq_t S = 1 ^ m;       /* sum 1 + m */
        result = poly_mul(result, S);
    }

    return result;
}
