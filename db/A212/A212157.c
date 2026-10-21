/*
 * A212157  Array T(n,k) = k^((prime(n)-1)/2) (mod prime(n)), n >= 2, k=1, 2,
 *          ... , prime(n)-1; T(1,1) = +1.
 * https://oeis.org/A212157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) gives the flattened array of T(r,k) where T(r,k) =
 *            k^((prime(r)−1)/2) (mod prime(r)), with k=1,…,prime(r)−1.
 *            a(1)=+1. For n>1, a(n)=+1 if the corresponding value ≡1 (mod
 *            prime(r)); otherwise a(n)=−1. The function is memoized but works
 *            in any call order
 * Range:     i8 n <= 66, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* returns uint32_t */
#include "../utils/mod_pow.h"     /* seq_t mod_pow(seq_t, uint64_t, seq_t) */

static idx_t *cumlen = NULL;         /* cumulative row lengths */
static uint32_t *primes_cache = NULL;/* list of primes */
static size_t  cap = 0;              /* current capacity of the arrays */
static idx_t   max_r = 0;            /* number of primes cached */

/* Ensure the arrays have at least new_cap elements. */
static void grow_cache(size_t new_cap) {
    if (new_cap <= cap)
        return;
    size_t new_cap2 = new_cap * 2 + 16;
    if (new_cap2 < new_cap)
        new_cap2 = new_cap;
    uint32_t *p_new = (uint32_t *)realloc(primes_cache, new_cap2 * sizeof(uint32_t));
    idx_t    *c_new = (idx_t    *)realloc(cumlen,    new_cap2 * sizeof(idx_t));
    if (!p_new || !c_new) {
        /* allocation failed – abort; this is acceptable for a contest setting */
        exit(1);
    }
    primes_cache = p_new;
    cumlen       = c_new;
    cap = new_cap2;
}

/* Ensure that primes and cumulative lengths are available up to the
   r‑th prime (1‑based).  After this, primes_cache[r-1] holds prime(r) and
   cumlen[r-1] the total number of terms up to row r. */
static void ensure_primes_upto(idx_t r) {
    while (max_r < r) {
        grow_cache(max_r + 1);
        uint32_t p = get_prime(max_r + 1);          /* prime(r) */
        primes_cache[max_r] = p;
        if (max_r == 0)
            cumlen[max_r] = (idx_t)(p - 1);
        else
            cumlen[max_r] = cumlen[max_r - 1] + (idx_t)(p - 1);
        ++max_r;
    }
}

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1; /* row 1, only one element +1 */

    /* Find the row r such that cumlen[r-1] < n <= cumlen[r] */
    ensure_primes_upto(max_r + 10);          /* be generous to cover n */
    idx_t r_idx = 0;
    while (cumlen[r_idx] < n)
        ++r_idx;                             /* r_idx is 0‑based */

    uint32_t p = primes_cache[r_idx];
    idx_t   prev_len = (r_idx == 0 ? (idx_t)0 : cumlen[r_idx - 1]);
    idx_t   k = n - prev_len;               /* 1‑based column index */

    /* Compute k^((p-1)/2) mod p */
    seq_t res = mod_pow((seq_t)k, (uint64_t)((p - 1) / 2), (seq_t)p);

    return (res == (seq_t)1) ? (seq_t)1 : (seq_t)-1;
}
