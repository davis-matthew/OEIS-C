/*
 * A172091  Triangle, read by rows, T(n,k,q) = c(k,q) + c(n-k,q) - c(n, q)
 *          where c(n,q) = Product_{j=1..n-1} ((q^(j+1) - 1)/(q-1)) and q = 2.
 * https://oeis.org/A172091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n‑th element of the triangle * T(r,k,2) where * r =
 *            floor((sqrt(8*n+1)-1)/2) * k = n - r*(r+1)/2 * T(r,k,2) = c(k,2)
 *            + c(r-k,2) - c(r,2) * * c(m,2) = product_{j=1}^{m-1}
 *            (2^{j+1}-1), * with c(0)=c(1)=1
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 36, i64 n <= 66, i128 n <= 136,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"

/* Integer square root for 64‑bit values (no external helper needed). */
static idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if ((unsigned __int128)mid * mid <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Cached values of c(m,2). */
static seq_t *c_cache = NULL;
static idx_t c_len   = 0;   /* number of pre‑computed c values */

/* Ensure that c_cache contains values up to at least n. */
static void ensure_c(idx_t n)
{
    if (c_len > n) return;

    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(c_cache, new_len * sizeof(seq_t));
    if (!new_cache) abort();

    /* Initialize new entries */
    for (idx_t i = c_len; i < new_len; ++i) {
        if (i <= 1) {
            new_cache[i] = 1;          /* c(0)=c(1)=1 */
        } else {
            seq_t prev = new_cache[i - 1];
            seq_t pow_two = pow2(i);        /* 2^i  */
            new_cache[i] = prev * (pow_two - 1);
        }
    }

    c_cache = new_cache;
    c_len   = new_len;
}

seq_t a(idx_t n)
{
    /* Determine row r and column k from linear index n */
    idx_t tmp   = (idx_t)((unsigned __int128)8 * n + 1);   /* 8*n+1 */
    idx_t s     = isqrt_u64(tmp);                          /* floor sqrt */
    idx_t r     = (s - 1) >> 1;                            /* floor((s-1)/2) */

    unsigned __int128 prod = (unsigned __int128)r * (r + 1) / 2;
    idx_t start = (idx_t)prod;
    idx_t k = n - start;   /* column index */

    /* Ensure c_cache has enough pre‑computed values */
    ensure_c(r);

    /* Compute T(r,k,2) = c(k)+c(r-k)-c(r) */
    seq_t val = c_cache[k] + c_cache[r - k] - c_cache[r];
    return val;
}
