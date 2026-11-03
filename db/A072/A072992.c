/*
 * A072992  Numbers n such that sigma(n) does not divide C(2n,n).
 * https://oeis.org/A072992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 5420, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9589 (2026-11-03)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A072992: numbers n such that sigma(n) does not divide C(2n,n) */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Utility to compute exponent of prime p in C(2n,n) using Legendre's formula. */
static unsigned exponent_in_binom(uint64_t n, uint64_t p)
{
    unsigned e = 0;
    uint64_t power = p;
    while (power <= 2 * n) {
        e += (2 * n) / power - 2 * (n / power);
        if (power > (2 * n) / p) break;  /* avoid overflow */
        power *= p;
    }
    return e;
}

/* Simple dynamic array to hold the sequence values. */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint64_t checked_upto = 0;

/* Grow the cache array if needed. */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
    if (!tmp) abort();          /* fatal, the caller cannot recover */
    seq_cache = tmp;
    seq_cap  = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined but safe guard */

    /* Compute terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        ++checked_upto;
        uint64_t m = checked_upto;

        /* Compute sigma(m).  It is returned as seq_t, but it always fits
           into 64 bits for the n range used in tests, so we can cast. */
        seq_t sigma_seq = sigma_of(m);
        uint64_t sigma   = (uint64_t)sigma_seq;   /* safe cast */

        /* Factor sigma.  Allocate a modest fixed-size buffer. */
        uint64_t fac[64];
        uint64_t exp[64];
        size_t cnt = 0;
        factor_u64(sigma, fac, exp, &cnt);

        /* Check if sigma divides C(2m,m). */
        bool divides = true;
        for (size_t i = 0; i < cnt; ++i) {
            unsigned e_needed = (unsigned)exp[i];
            unsigned e_binom  = exponent_in_binom(m, fac[i]);
            if (e_binom < e_needed) {
                divides = false;
                break;
            }
        }

        if (!divides) {
            /* This m is a term of the sequence. */
            ensure_capacity(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)m;
        }
    }

    return seq_cache[(size_t)n - 1];
}
