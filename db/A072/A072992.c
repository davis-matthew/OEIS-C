/*
 * A072992  Numbers n such that sigma(n) does not divide C(2n,n).
 * https://oeis.org/A072992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 5420, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   exponent_in_binom, factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A072992: numbers n such that sigma(n) does not divide C(2n,n) */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"
#include "../utils/exponent_in_binom.h"

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
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
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
