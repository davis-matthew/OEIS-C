/*
 * A176361  G.f.: exp( Sum_{n>=1} sigma(n*2^(n-1))*x^n/n ).
 * https://oeis.org/A176361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 1, a(n) = (1/n) * Σ_{k=1}^n σ(k·2^{k-1}) * a(n-k),
 * where σ(m) = sum of divisors of m.
 *
 * The implementation uses a straight‑forward recurrence and
 * precomputes the divisor–sum values needed for σ(k·2^{k-1}).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* ------------------------------------------------------------------ */
/* Cache for the divisor–sum function σ(·) up to a maximum value.     */
/* ------------------------------------------------------------------ */
static seq_t *sigma_cache = NULL;
static size_t sigma_cap = 0;

/* ------------------------------------------------------------------ */
/* Cache for s_k = σ(k·2^{k-1}) (index from 1).                         */
/* ------------------------------------------------------------------ */
static seq_t *s_cache = NULL;
static size_t s_cap = 0;

/* ------------------------------------------------------------------ */
/* Cache for the sequence a(n) (index from 0).                         */
/* ------------------------------------------------------------------ */
static seq_t *a_cache = NULL;
static size_t a_len = 0;

/* ------------------------------------------------------------------ */
/* Ensures sigma_cache contains σ(i) for all 0 <= i <= need.          */
/* ------------------------------------------------------------------ */
static void ensure_sigma(size_t need)
{
    if (sigma_cap > need) return;               /* already sufficient */

    /* free old cache */
    free(sigma_cache);

    /* allocate new cache (indices 0..need) */
    sigma_cache = (seq_t*)calloc(need + 1, sizeof(seq_t));
    if (!sigma_cache) abort();

    /* divisor–sum sieve */
    for (size_t i = 1; i <= need; ++i) {
        for (size_t j = i; j <= need; j += i) {
            sigma_cache[j] += (seq_t)i;
        }
    }
    sigma_cap = need + 1;
}

/* ------------------------------------------------------------------ */
/* Ensures s_cache contains σ(k·2^{k-1}) for all 0 <= k <= need.      */
/* ------------------------------------------------------------------ */
static void ensure_s(size_t need)
{
    if (s_cap > need) return;               /* already good */

    /* σ(k·2^{k-1}) requires σ of odd parts up to need. */
    ensure_sigma(need);

    free(s_cache);
    s_cache = (seq_t*)calloc(need + 1, sizeof(seq_t));
    if (!s_cache) abort();

    s_cache[0] = (seq_t)0;                  /* unused (k starts at 1) */

    for (size_t k = 1; k <= need; ++k) {
        /* Extract odd part of k and its exponent of 2. */
        size_t t = k;
        size_t e = 0;
        while ((t & 1ULL) == 0ULL) {
            ++e;
            t >>= 1;
        }
        size_t odd_part = t;                /* k >> e */

        seq_t sigma_odd = sigma_cache[odd_part];

        /* σ(2^{e + k - 1}) = 2^{e + k} - 1 */
        seq_t two_pow = pow2((idx_t)(e + k));
        seq_t two_part = two_pow - (seq_t)1;

        s_cache[k] = two_part * sigma_odd;
    }
    s_cap = need + 1;
}

/* ------------------------------------------------------------------ */
/* Ensures a_cache contains a(n) for all 0 <= n <= need.               */
/* ------------------------------------------------------------------ */
static void ensure_a(size_t need)
{
    if (a_len > need) return;               /* already set */

    ensure_s(need);

    /* Allocate or grow a_cache to size need+1 */
    a_cache = (seq_t*)realloc(a_cache, (need + 1) * sizeof(seq_t));
    if (!a_cache) abort();

    /* Initialize new entries to zero (they will be overwritten). */
    for (size_t i = a_len; i <= need; ++i) {
        a_cache[i] = (seq_t)0;
    }

    /* First term a(0) = 1 */
    if (a_len == 0) {
        a_cache[0] = (seq_t)1;
        a_len = 1;
    }

    /* Compute missing values using the recurrence */
    for (size_t n = a_len; n <= need; ++n) {
        seq_t sum = (seq_t)0;
        for (size_t k = 1; k <= n; ++k) {
            sum += s_cache[k] * a_cache[n - k];
        }
        a_cache[n] = sum / (seq_t)n;      /* division is exact by theory */
    }
    a_len = need + 1;
}

/* ------------------------------------------------------------------ */
/* Public function                                                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Guard against unsigned underflow (n == –1) */
    if ((idx_t)-1 == n) return (seq_t)0;

    ensure_a((size_t)n);
    return a_cache[(size_t)n];
}
