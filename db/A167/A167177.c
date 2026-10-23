/*
 * A167177  Expansion of 1/((1 +x +x^2)^2 *(1 +x^2 +x^3)^3).
 * https://oeis.org/A167177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 34, i32 n <= 83, i64 n <= 190,
 *            i128 n <= 415, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A167177
 * 
 * From the definition: 1/((1 +x +x^2)^2 *(1 +x^2 +x^3)^3).
 * The integer coefficients satisfy the linear recurrence
 * a(n) = -2*a(n-1) -6*a(n-2) -11*a(n-3) -19*a(n-4) -27*a(n-5)
 *        -34*a(n-6) -38*a(n-7) -36*a(n-8) -30*a(n-9) -21*a(n-10)
 *        -12*a(n-11) -5*a(n-12) -a(n-13),  n ≥ 13.
 * Initial values (a(0) … a(13)) are listed below. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- primitive cache data ---------------------------------- */
static seq_t *cache     = NULL;   /* dynamic array holding computed terms   */
static idx_t cache_len = 0;       /* number of already computed terms      */
static size_t cache_cap = 0;      /* allocated capacity (in seq_t's)       */

/* ---------- initial 14 values ------------------------------------- */
static const seq_t init_vals[14] = {
    (seq_t)  1,  (seq_t)-2, (seq_t)-2,  (seq_t) 5,  (seq_t) 5,
    (seq_t) -7, (seq_t)-13, (seq_t) 2,  (seq_t)29, (seq_t)19,
    (seq_t)-47, (seq_t)-68, (seq_t)43, (seq_t)151
};

/* ---------- grow or create the cache -------------------------------- */
static void ensure_cache(idx_t n)
{
    /* No work if we already have enough values.  */
    if (n < cache_len) return;

    /* Determine how many values we need to store.  */
    size_t needed = (size_t)(n + 1);

    /* Allocate or grow the storage.  */
    if (!cache) {                                  /* first allocation   */
        cache_cap = needed > 14 ? needed : 14;     /* at least hold 14   */
        cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);

        /* Copy the initial 14 terms (if required).  */
        size_t i;
        for (i = 0; i < 14 && i < needed; ++i) cache[i] = init_vals[i];
        cache_len = i;                          /* number of initialized terms   */

    } else if (needed > cache_cap) {              /* grow existing array   */
        cache_cap = needed;
        cache     = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }

    /* If we still do not have enough elements, fill them using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        if (i < 14) {                       /* should never happen for i>=cache_len */
            cache[i] = init_vals[(size_t)i];
            continue;
        }
        /* apply the 13-term linear recurrence */
        sum -= (seq_t)2  * cache[i - 1];
        sum -= (seq_t)6  * cache[i - 2];
        sum -= (seq_t)11 * cache[i - 3];
        sum -= (seq_t)19 * cache[i - 4];
        sum -= (seq_t)27 * cache[i - 5];
        sum -= (seq_t)34 * cache[i - 6];
        sum -= (seq_t)38 * cache[i - 7];
        sum -= (seq_t)36 * cache[i - 8];
        sum -= (seq_t)30 * cache[i - 9];
        sum -= (seq_t)21 * cache[i -10];
        sum -= (seq_t)12 * cache[i -11];
        sum -= (seq_t)5  * cache[i -12];
        sum -=           cache[i -13];
        cache[i] = sum;
    }
    cache_len = n + 1;        /* now we have terms 0 … n */
}

/* ---------- the required sequence function ------------------------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
