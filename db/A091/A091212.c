/*
 * A091212  Composite numbers whose binary representation encodes a polynomial
 *          reducible over GF(2).
 * https://oeis.org/A091212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 186, u16 n <= 9375, u32 n <= 9375, u64 n <= 9375,
 *            u128 n <= 9375, big any n
 * Helpers:   bitlen_seq_t, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A091212: composite numbers whose binary representation encodes a polynomial
 * reducible over GF(2).  The function searches composite integers in order,
 * tests each for reducibility over GF(2) by dividing by all previously
 * found irreducible polynomials up to degree floor(deg/2), and caches the
 * results.  The cache allows repeated calls with arbitrary n. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime.h"
#include "../utils/bitlen_seq_t.h"

/* ---------  Iron-irreducible polynomials (monic) --------------------- */
static seq_t *irr_list = NULL;        /* integer representation of the polynomial */
static idx_t *irr_deg  = NULL;        /* degree of each polynomial in irr_list */
static idx_t irr_count = 0;           /* number of irreducibles stored */
static idx_t irr_max_deg = 0;         /* maximum degree currently generated */

/* Ensure we have all monic irreducible polynomials of degree ≤ max_deg.
 * We generate them by testing each candidate polynomial of degree d
 * against all already known irreducibles of lower degree. */
static void
ensure_irreducibles(idx_t max_deg)
{
    if (max_deg <= irr_max_deg)
        return;

    /* Allocate or resize the storage arrays if needed. */
    if (irr_list == NULL) {
        irr_list = (seq_t *)malloc(16 * sizeof(seq_t));
        irr_deg  = (idx_t *)malloc(16 * sizeof(idx_t));
        if (!irr_list || !irr_deg) {
            abort();                 /* allocation failure */
        }
        irr_count = 0;
        irr_max_deg = 0;
    }

    size_t cap = irr_count ? irr_count : 16;
    while (irr_max_deg < max_deg) {
        irr_max_deg++;
        if (irr_count == cap) {
            cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(irr_list, cap * sizeof(seq_t));
            if (!tmp) abort();
            irr_list = tmp;
            idx_t *tmpd = (idx_t *)realloc(irr_deg, cap * sizeof(idx_t));
            if (!tmpd) abort();
            irr_deg = tmpd;
        }

        idx_t d = irr_max_deg;
        seq_t start = (seq_t)1 << d;        /* 1 followed by d zero bits */
        seq_t end   = (seq_t)1 << (d + 1);  /* one higher degree */

        for (seq_t cand = start; cand < end; ++cand) {
            bool is_irred = true;

            /* Try dividing by all known irreducibles of degree ≤ d/2. */
            for (idx_t j = 0; j < irr_count; ++j) {
                if (irr_deg[j] > d / 2)
                    break;                    /* later ones have larger degree */

                /* GF(2) division using a simple shift‑and‑xor loop. */
                seq_t r = cand;
                idx_t df = bitlen_seq_t(r) - 1;
                idx_t dg = irr_deg[j];
                while (df >= dg) {
                    r ^= irr_list[j] << (df - dg);
                    if (!r) break;
                    df = bitlen_seq_t(r) - 1;
                }
                if (r == 0) {                 /* divisible => not irreducible */
                    is_irred = false;
                    break;
                }
            }

            if (is_irred) {
                irr_list[irr_count]  = cand;
                irr_deg[irr_count]   = d;
                irr_count++;
            }
        }
    }
}

/* GF(2) remainder of f divided by g (g != 0). */
static seq_t
gf2_mod(seq_t f, seq_t g)
{
    idx_t df = bitlen_seq_t(f) - 1;
    idx_t dg = bitlen_seq_t(g) - 1;
    if (dg > df)
        return f;

    seq_t r = f;
    while (df >= dg) {
        r ^= g << (df - dg);
        if (!r)
            break;
        df = bitlen_seq_t(r) - 1;
    }
    return r;
}

/* Test whether the binary polynomial of a composite m is reducible
 * over GF(2).  Returns true iff m is composite and its polynomial
 * has a non‑trivial divisor. */
static bool
is_reducible(idx_t m)
{
    if (m < 4)                 /* 0,1,2,3 are not composite */
        return false;

    idx_t deg = bitlen_seq_t((seq_t)m) - 1;
    ensure_irreducibles(deg / 2);

    for (idx_t j = 0; j < irr_count; ++j) {
        if (irr_deg[j] > deg / 2)
            break;
        if (gf2_mod((seq_t)m, irr_list[j]) == 0)
            return true;        /* divisible by an irreducible */
    }
    return false;
}

/* ------------------  Caching of sequence terms --------------------- */
static seq_t *cache = NULL;     /* cached values of a(1), a(2), ... */
static idx_t cache_len = 0;     /* number of cached values */
static idx_t cache_cap = 0;     /* allocated capacity for cache */
static idx_t next_candidate = 4;/* next number to test for the sequence */

/* Ensure that cache has room for at least n elements (index n starts at 1). */
static void
ensure_cache(idx_t n)
{
    if (cache_cap >= n)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) {
        abort();                 /* allocation failure */
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)                 /* OEIS defined for n ≥ 1 only */
        return 0;

    ensure_cache(n);

    while (cache_len < n) {
        if (next_candidate < 4)
            next_candidate = 4;

        if (!is_prime(next_candidate) && is_reducible(next_candidate)) {
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
