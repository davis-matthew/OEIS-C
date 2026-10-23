/*
 * A174161  a(1) = 2. Let k >= 1 be the minimal integer such that 2*k*a(n-1) -
 *          1 has at least one prime divisor which is not already in the
 *          sequence. Then a(n) is the smallest such divisor.
 * https://oeis.org/A174161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined recursively. a(1) = 2. For n ≥ 2 let k be the
 *            smallest positive integer such that 2*k*a(n-1) – 1 has at least
 *            one prime divisor that has not appeared in the sequence yet.
 *            Then a(n) is the smallest such new prime divisor. The sequence
 *            is a permutation of the prime numbers
 * Range:     u8 n <= 13, u16 n <= 158, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External utility needed for 64‑bit factoring. */
#include "../utils/factor_u64.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Dynamic array helpers for seq_t values. */
static seq_t *seq_buf = NULL;          /* holds the sequence */
static size_t seq_size = 0;            /* allocated capacity */
static size_t seq_used = 0;            /* number of terms stored */

/* Dynamic array helpers for used primes as uint64_t. */
static uint64_t *used_buf = NULL;
static size_t used_size = 0;
static size_t used_used = 0;

/* Ensure seq_buf has at least capacity 'need'. */
static void seq_grow(size_t need) {
    if (seq_buf == NULL) {
        seq_buf = (seq_t *)malloc((need > 8 ? need : 8) * sizeof(seq_t));
        seq_size = (need > 8 ? need : 8);
    } else if (need > seq_size) {
        seq_buf = (seq_t *)realloc(seq_buf, need * sizeof(seq_t));
        seq_size = need;
    }
}

/* Ensure used_buf has at least capacity 'need'. */
static void used_grow(size_t need) {
    if (used_buf == NULL) {
        used_buf = (uint64_t *)malloc((need > 8 ? need : 8) * sizeof(uint64_t));
        used_size = (need > 8 ? need : 8);
    } else if (need > used_size) {
        used_buf = (uint64_t *)realloc(used_buf, need * sizeof(uint64_t));
        used_size = need;
    }
}

/* Linear search whether a prime p is already in the used set. */
static bool used_contains(uint64_t p) {
    for (size_t i = 0; i < used_used; ++i)
        if (used_buf[i] == p) return true;
    return false;
}

/* Append a new used prime. */
static void used_add(uint64_t p) {
    used_grow(used_used + 1);
    used_buf[used_used++] = p;
}

/* Initialize the first term of the sequence. */
static void init_sequence(void) {
    if (seq_used != 0) return;
    seq_grow(1);
    seq_buf[0] = (seq_t)2;
    seq_used = 1;
    used_grow(1);
    used_buf[0] = 2;
    used_used = 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, but harmless */
        return (seq_t)0;

    init_sequence();

    /* Grow the sequence up to the requested index. */
    while (seq_used < (size_t)n) {
        /* previous term */
        seq_t prev = seq_buf[seq_used - 1];
        uint64_t prev64 = (uint64_t)prev;

        /* Search for the minimal k. */
        for (uint64_t k = 1; ; ++k) {
            /* Compute m = 2*k*prev - 1, using 128‑bit to avoid overflow. */
            __uint128_t tmp = (__uint128_t)2 * k * prev64 - 1;
            if (tmp > (uint64_t)(-1)) {
                /* Too large for 64‑bit factoring – this should not happen for the tested range. */
                continue;
            }
            uint64_t m = (uint64_t)tmp;

            /* Factor m completely (only 64‑bit). */
            static uint64_t primes[128];
            static uint64_t exps[128];
            size_t cnt = 0;
            factor_u64(m, primes, exps, &cnt);

            /* Find the smallest new prime divisor. */
            uint64_t best = 0;
            for (size_t i = 0; i < cnt; ++i) {
                uint64_t p = primes[i];
                if (!used_contains(p)) {
                    if (best == 0 || p < best)
                        best = p;
                }
            }

            if (best != 0) {
                /* We found the next term. Store it. */
                seq_grow(seq_used + 1);
                seq_buf[seq_used++] = (seq_t)best;
                used_add(best);
                break;
            }
            /* If no new prime found for this k, try next k. */
        }
    }

    return seq_buf[n - 1];
}
