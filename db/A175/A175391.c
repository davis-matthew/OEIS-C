/*
 * A175391  Perfect squares having a square number of divisors.
 * https://oeis.org/A175391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A063774(n)^2 (perfect squares whose number of divisors is
 *            a square)
 * Range:     u8 n <= 5, u16 n <= 86, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Utilities for arbitrary‑precision or built‑in integer types */
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */
#include "../utils/seq_mul.h"      /* seq_mul(seq_t, seq_t) */

/* Prime factorisation helpers -------------------------------------------- */

static uint32_t *spf = NULL;          /* smallest prime factor table */
static size_t   spf_limit = 0;        /* current size of spf table */

/* Saturated linear sieve to build spf up to at least `limit` */
static void
ensure_spf(idx_t limit)
{
    if (limit <= spf_limit) return;

    size_t new_lim = limit + 1000;              /* grow a bit beyond */
    uint32_t *new_spf = (uint32_t *)calloc(new_lim + 1, sizeof(uint32_t));
    if (!new_spf) exit(1);

    /* First pass: initialise primes and spf */
    for (size_t i = 2; i <= new_lim; ++i) {
        if (new_spf[i] == 0) {                 /* i is prime */
            new_spf[i] = (uint32_t)i;
            for (size_t j = i * i; j <= new_lim; j += i)
                if (new_spf[j] == 0)
                    new_spf[j] = (uint32_t)i;
        }
    }

    free(spf);
    spf = new_spf;
    spf_limit = new_lim;
}

/* ------------------------------------------------------------------------ */

/* Cached sequence values -------------------------------------------------- */
static seq_t *seq_vals = NULL;   /* storage of a(i) = m^2 */
static size_t seq_len = 0;       /* number of terms stored */
static size_t seq_cap = 0;       /* capacity of seq_vals */

/* Current m processed; start from 0 so that first increment gives m=1 */
static idx_t m_current = 0;

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but return 0 */

    /* Grow cache until we have at least n terms */
    while (seq_len < (size_t)n) {
        ++m_current;

        /* Ensure prime data covers current m */
        if ((idx_t)spf_limit < m_current + 1)
            ensure_spf((idx_t) (m_current * 2 + 1));

        /* Compute τ(m²) = Π (2·eᵢ + 1) from factorisation of m */
        seq_t tau = (seq_t)1;
        idx_t x = m_current;
        while (x > 1) {
            uint32_t p = spf[x];
            int e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            tau = seq_mul(tau, (seq_t)(2 * e + 1));
        }

        /* Check if τ(m²) is a perfect square */
        seq_t sq = floor_sqrt(tau);
        if (seq_mul(sq, sq) == tau) {
            /* Store m² in the cache */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 4;
                seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
                if (!seq_vals) exit(1);
            }
            seq_t m_seq = (seq_t)m_current;
            seq_vals[seq_len++] = seq_mul(m_seq, m_seq);
        }
    }

    return seq_vals[n - 1];
}
