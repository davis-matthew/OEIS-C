/*
 * A078995  a(n) = Sum_{k=0..n} C(4*k,k)*C(4*(n-k),n-k).
 * https://oeis.org/A078995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*k,k)*binomial(4*(n-k),n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Computes the OEIS sequence A078995.  Uses memoisation and
   a cached table of the binomial factors b(k) = binomial(4*k,k). */
seq_t a(idx_t n)
{
    /* Tables holding already computed values.
       a_cache[i] = a(i),  b_cache[k] = binomial(4*k,k)       */
    static seq_t *a_cache = NULL;  /* a(n) values         */
    static size_t a_cap   = 0;      /* current capacity    */
    static size_t a_len   = 0;      /* number of terms stored */

    static seq_t *b_cache = NULL;  /* b(k) = binomial(4*k,k) */
    static size_t b_cap   = 0;
    static size_t b_len   = 0;

    /* ---------- extend b_cache to n ---------- */
    if ((idx_t)(n + 1) > (idx_t)b_cap) {
        size_t new_cap = (b_cap == 0) ? 1 : b_cap;
        while (new_cap <= (size_t)(n + 1)) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        b_cache = tmp;
        b_cap   = new_cap;
    }
    for (size_t k = b_len; k <= (size_t)n; ++k) {
        b_cache[k] = binomial((idx_t)(4 * k), (idx_t)k);
    }
    b_len = (size_t)n + 1;     /* now b_cache[0..n] are valid */

    /* ---------- extend a_cache to n ---------- */
    if ((idx_t)(n + 1) > (idx_t)a_cap) {
        size_t new_cap = (a_cap == 0) ? 1 : a_cap;
        while (new_cap <= (size_t)(n + 1)) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        a_cache = tmp;
        a_cap   = new_cap;
    }

    /* ---------- compute missing a values ---------- */
    for (size_t i = a_len; i <= (size_t)n; ++i) {
        seq_t sum = (seq_t)0;
        for (size_t k = 0; k <= i; ++k) {
            sum += b_cache[k] * b_cache[i - k];
        }
        a_cache[i] = sum;
    }
    a_len = (size_t)n + 1;

    return a_cache[(size_t)n];
}
