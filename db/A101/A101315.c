/*
 * A101315  Primes p such that p+2, p^2 - 2p + 2, and p^2 - 2p + 4 are all
 *          prime.
 * https://oeis.org/A101315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 51 terms through n = 9043 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A101315: primes p with p + 2, p^2 - 2p + 2 and p^2 - 2p + 4 all prime. The
 * primes are streamed in order by a segmented sieve over odd numbers; at each
 * twin pair (p, p+2) the two quadratics are tested with the exact 64-bit
 * primality test. Terms are cached and the stream resumes. (The original ran a
 * Miller-Rabin test on every integer and ran out of time.) */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"

#define SEG (1u << 20)                    /* odd numbers per segment */

static uint64_t *terms = NULL;
static size_t term_len = 0, term_cap = 0;

static uint32_t *base = NULL;             /* odd primes up to sqrt of the sieved range */
static size_t nbase = 0;
static uint64_t base_lim = 0;
static uint64_t seg_lo = 3;               /* next odd number to sieve */

static void grow_base(uint64_t lim)
{
    unsigned char *c = (unsigned char *)calloc((size_t)lim + 1, 1);
    if (!c) abort();
    free(base);
    nbase = 0;
    base = (uint32_t *)malloc(((size_t)lim / 2 + 1) * sizeof *base);
    if (!base) abort();
    for (uint64_t i = 3; i <= lim; i += 2) {
        if (c[i]) continue;
        base[nbase++] = (uint32_t)i;
        for (uint64_t j = i * i; j <= lim; j += 2 * i) c[j] = 1;
    }
    free(c);
    base_lim = lim;
}

static void push_term(uint64_t v)
{
    if (term_len == term_cap) {
        term_cap = term_cap ? 2 * term_cap : 4096;
        uint64_t *t = (uint64_t *)realloc(terms, term_cap * sizeof *t);
        if (!t) abort();
        terms = t;
    }
    terms[term_len++] = v;
}

static uint64_t last_p = 0;

static void next_prime(uint64_t p)
{
    /* a twin pair (last_p, p): test the two quadratics of last_p */
    if (p == last_p + 2) {
        uint64_t q = last_p;
        if (is_prime_u64(q * q - 2 * q + 2) && is_prime_u64(q * q - 2 * q + 4)) push_term(q);
    }
    last_p = p;
}

static void sieve_segment(void)
{
    static unsigned char comp[SEG];
    const uint64_t lo = seg_lo, hi = lo + 2 * (uint64_t)SEG;   /* odd numbers lo, lo+2, ... < hi */
    while (base_lim * base_lim < hi) grow_base(base_lim ? 2 * base_lim : 1 << 16);
    memset(comp, 0, sizeof comp);
    for (size_t i = 0; i < nbase; ++i) {
        uint64_t p = base[i];
        if (p * p >= hi) break;
        uint64_t s = p * p;
        if (s < lo) {
            s = (lo + p - 1) / p * p;
            if (!(s & 1)) s += p;
        }
        for (uint64_t j = s; j < hi; j += 2 * p) comp[(j - lo) / 2] = 1;
    }
    for (uint32_t i = 0; i < SEG; ++i)
        if (!comp[i]) next_prime(lo + 2 * (uint64_t)i);
    seg_lo = hi;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (!last_p) next_prime(2);
    while (term_len < (size_t)n) sieve_segment();
    return (seq_t)terms[n - 1];
}
