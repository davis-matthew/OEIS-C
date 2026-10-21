/*
 * A082393  Let p = n-th prime of the form 4k+1, take the integer solution
 *          (x,y) to the Pellian equation x^2 - p*y^2 = 1 with the smallest y
 *          >= 1; sequence gives value of y.
 * https://oeis.org/A082393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 36, u128 n <= 82,
 *            big any n
 * Helpers:   is_prime, isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A082393:  a(n) = y, the minimal positive y in the Pell equation
 *   x^2 – p*y^2 = 1,
 *   where p is the n‑th prime of the form 4k+1.
 *   The sequence begins 4,180,8,1820,12,320,…   (offset 1).  */

/*  Required utilities */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>          /* malloc, realloc, free */
#include "../utils/is_prime.h"
#include "../utils/isqrt_seq.h"

static uint64_t *prime1_list = NULL;
static size_t prime1_cap   = 0;
static size_t prime1_cnt   = 0;

/* Append a new prime to the cache */
static void append_prime1(uint64_t p)
{
    if (prime1_cnt >= prime1_cap) {
        prime1_cap = prime1_cap ? prime1_cap * 2 : 256;
        prime1_list = (uint64_t *)realloc(prime1_list,
                                          prime1_cap * sizeof(uint64_t));
    }
    prime1_list[prime1_cnt++] = p;
}

/* Generate primes of the form 4k+1 up to at least the n‑th one.  */
static void ensure_primes1(idx_t n)
{
    if (prime1_cnt >= (size_t)n) return;

    uint64_t last = prime1_cnt ? prime1_list[prime1_cnt - 1] : 2;
    while (prime1_cnt < (size_t)n) {
        uint64_t cand = last + 1;
        for (;;) {
            if (cand % 4 == 1 && is_prime(cand)) {
                append_prime1(cand);
                last = cand;
                break;
            }
            ++cand;
        }
    }
}

/* Cache for y values */
static seq_t *y_cache  = NULL;
static size_t  y_cap   = 0;
static size_t  y_cnt   = 0;

/* Make cache large enough for index n (1‑based).   */
static void ensure_y_cache(idx_t n)
{
    if (y_cnt >= (size_t)n) return;
    size_t cap = y_cap ? y_cap * 2 : 256;
    while (cap < (size_t)n) cap <<= 1;
    y_cache = (seq_t *)realloc(y_cache, cap * sizeof(seq_t));
    for (size_t i = y_cnt; i < cap; ++i) y_cache[i] = (seq_t)0;
    y_cap = cap; y_cnt = cap;
}

/* Compute the minimal y for a given prime D (=p).  */
static seq_t fundamental_y(idx_t D)
{
    /* Continued fraction coefficients for sqrt(D) */
    idx_t a0 = (idx_t)isqrt_seq(D);
    idx_t m = 0, d = 1, a = a0;
    /* Period coefficients stored in a fixed array */
    idx_t coeff[2048];
    size_t L = 0;
    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        coeff[L++] = a;
    } while (a != 2 * a0 && L < sizeof(coeff)/sizeof(coeff[0]));

    /* Index of fundamental solution:  k = L-1 if L even, 2*L-1 otherwise */
    size_t k = (L % 2 == 0) ? (L - 1) : (2 * L - 1);

    /* Build convergents up to index k.  */
    seq_t p_m2 = (seq_t)1;
    seq_t p_m1 = (seq_t)a0;
    seq_t q_m2 = (seq_t)0;
    seq_t q_m1 = (seq_t)1;

    for (size_t i = 1; i <= k; ++i) {
        idx_t coef = coeff[(i - 1) % L];
        seq_t p = (seq_t)coef * p_m1 + p_m2;
        seq_t q = (seq_t)coef * q_m1 + q_m2;

        p_m2 = p_m1; p_m1 = p;
        q_m2 = q_m1; q_m1 = q;
    }
    return q_m1;   /* y is the denominator of the convergent */
}

/* Compute the cached y value for the n‑th prime of the form 4k+1.  */
static seq_t get_y_cached(idx_t n)
{
    ensure_y_cache(n);
    if (y_cache[n - 1] == (seq_t)0) {
        idx_t p = prime1_list[n - 1];
        y_cache[n - 1] = fundamental_y(p);
    }
    return y_cache[n - 1];
}

/* Public function required by the project.   */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset zero   */
    ensure_primes1(n);                    /* make sure we have the prime  */
    return get_y_cached(n);
}
