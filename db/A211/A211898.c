/*
 * A211898  G.f.: exp( Sum_{n>=1} (2^n - (-1)^n)^n * x^n/n ).
 * https://oeis.org/A211898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A211898:
 *  a(0) = 1
 *  a(n) = (1/n) * Σ_{k=1..n} (2^k - (-1)^k)^k * a(n-k)   (n ≥ 1)
 *  where (2^k - (-1)^k)^k is evaluated with arbitrary precision.
 *
 * This implementation caches all computed values of a(n) and the intermediate
 * terms T(k) = (2^k - (-1)^k)^k for efficient repeated queries.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/pow2.h"
#include "../utils/big_pow.h"

/* Forward declaration as required by the build system. */
seq_t a(idx_t n);

/* ---------- caching of T(k) = (2^k - (-1)^k)^k ---------- */

static seq_t *T_cache = NULL;      /*!< cache for T(k), k = 0.. */
static size_t T_cap  = 0;          /*!< allocated array size (>= needed)   */
static size_t T_len  = 0;          /*!< number of cached entries (next free index) */

/* Ensure T_cache can hold indices up to n (inclusive). */
static void ensure_T(idx_t n)
{
    size_t new_cap = (size_t)(n + 1);
    if (new_cap > T_cap) {
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (T_cache) {
            memcpy(new_arr, T_cache, T_cap * sizeof(seq_t));
            free(T_cache);
        }
        T_cache = new_arr;
        T_cap   = new_cap;
    }
}

/* Compute T(k) for all required indices up to n. */
static void compute_T_up_to(idx_t n)
{
    if (T_len > n) return;          /* already done */
    if (T_len == 0) {               /* first time: allocate at least index 0 */
        ensure_T(1);
        T_cache[0] = 0;             /* unused slot, just in case */
        T_len     = 1;
    }
    ensure_T(n);
    for (idx_t k = (idx_t)T_len; k <= n; ++k) {
        seq_t base = pow2(k);       /* 2^k */
        if ((k & 1) == 0) {         /* even k : subtract 1   */
            base -= (seq_t)1;
        } else {                    /* odd k  : add 1        */
            base += (seq_t)1;
        }
        T_cache[k] = big_pow(base, (seq_t)k);
    }
    T_len = (size_t)(n + 1);
}

/* ---------- caching of a(n) ---------- */

static seq_t *a_cache = NULL;      /*!< cache for a(n), n = 0.. */
static size_t a_cap  = 0;          /*!< allocated array size (>= needed)   */
static size_t a_len  = 0;          /*!< number of cached entries (next free index) */

/* Ensure a_cache can hold indices up to n (inclusive). */
static void ensure_a(idx_t n)
{
    size_t new_cap = (size_t)(n + 1);
    if (new_cap > a_cap) {
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (a_cache) {
            memcpy(new_arr, a_cache, a_cap * sizeof(seq_t));
            free(a_cache);
        }
        a_cache = new_arr;
        a_cap   = new_cap;
    }
}

/* Compute a(n) for all required indices up to n. */
static void compute_a_up_to(idx_t n)
{
    if (a_len > n) return;          /* already done */
    ensure_a(n);
    for (idx_t i = (idx_t)a_len; i <= n; ++i) {
        if (i == 0) {                /* a(0) = 1 */
            a_cache[0] = (seq_t)1;
            continue;
        }
        /* Ensure all T(k) needed for this i are ready. */
        compute_T_up_to(i);

        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            /* Multiply a(i-k) by T(k) and add to sum. */
            sum += a_cache[i - k] * T_cache[k];
        }
        /* a(i) = sum / i  (division is exact). */
        a_cache[i] = sum / (seq_t)i;
    }
    a_len = (size_t)(n + 1);
}

/* ---------- public interface ---------- */

seq_t a(idx_t n)
{
    /* n may be larger than 0;  no special treatment of negative values. */
    compute_a_up_to(n);
    return a_cache[n];
}
