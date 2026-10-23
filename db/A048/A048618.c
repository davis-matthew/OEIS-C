/*
 * A048618  Even numbers n such that binomial(n,n/2) is divisible by n/2.
 * https://oeis.org/A048618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth even integer m = 2k such that * binomial(m, m/2)
 *            is divisible by k. In terms of k, the condition * holds iff, for
 *            every prime divisor p of k, the exponent of p in * binomial(2k,
 *            k) is at least its exponent in k
 * Range:     u8 n <= 15, u16 n <= 3610, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  SPDX-License-Identifier: BSD-3-Clause  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Extend a smallest‑prime‑factor sieve up to new_limit (new_limit > limit). */
static void extend_sieve(uint64_t **spf, size_t *limit, size_t new_limit)
{
    uint64_t *new_spf;
    size_t old_lim = *limit;

    if (*spf == NULL) {
        new_spf = (uint64_t*)malloc((new_limit + 1) * sizeof(uint64_t));
        if (!new_spf) exit(EXIT_FAILURE);
        memset(new_spf, 0, (new_limit + 1) * sizeof(uint64_t));
    } else {
        new_spf = (uint64_t*)realloc(*spf, (new_limit + 1) * sizeof(uint64_t));
        if (!new_spf) exit(EXIT_FAILURE);
        memset(new_spf + old_lim + 1, 0, (new_limit - old_lim) * sizeof(uint64_t));
    }

    size_t start = old_lim ? old_lim + 1 : 2;
    for (uint64_t i = start; i <= new_limit; ++i) {
        if (!new_spf[i]) {
            new_spf[i] = i;
            if (i <= new_limit / i) {
                for (uint64_t j = i * i; j <= new_limit; j += i)
                    if (!new_spf[j]) new_spf[j] = i;
            }
        }
    }

    *spf   = new_spf;
    *limit = new_limit;
}

/* Exponent of prime p in C(2k,k). */
static unsigned exp_in_binom(uint64_t k, uint64_t p)
{
    unsigned sum = 0;
    uint64_t power = p;
    uint64_t two_k = 2 * k;

    while (power <= two_k) {
        sum += two_k / power - 2 * (k / power);
        if (power > two_k / p) break;
        power *= p;
    }
    return sum;
}

/* Return the n-th term of sequence A048618. */
extern seq_t a(idx_t n)
{
    static seq_t *values = NULL;
    static size_t count  = 0;
    static size_t cap    = 0;

    static uint64_t *spf    = NULL;
    static size_t   spf_lim = 0;
    static uint64_t next_k  = 1;      /* current k to test */

    /* Grow as needed. */
    while (count < (size_t)n) {
        /* Ensure sieve covers next_k. */
        if (next_k > spf_lim) {
            size_t new_lim = spf_lim ? spf_lim * 2 : 200000;
            if (new_lim < next_k) new_lim = next_k * 2;
            extend_sieve(&spf, &spf_lim, new_lim);
        }

        uint64_t k = next_k++;
        bool ok = true;

        /* Factorise k and check condition for each prime factor. */
        uint64_t temp = k;
        while (temp > 1) {
            uint64_t p   = spf[temp];
            unsigned ek = 0;
            while (temp % p == 0) {
                temp /= p;
                ++ek;
            }
            if (exp_in_binom(k, p) < ek) {
                ok = false;
                break;
            }
        }

        if (ok) {
            /* Store m = 2k. */
            if (count == cap) {
                size_t new_cap = cap ? 2 * cap : 64;
                seq_t *new_arr = (seq_t*)realloc(values, new_cap * sizeof(seq_t));
                if (!new_arr) exit(EXIT_FAILURE);
                values = new_arr;
                cap = new_cap;
            }
            values[count++] = (seq_t)(2 * k);
        }
    }

    return values[n - 1];
}
