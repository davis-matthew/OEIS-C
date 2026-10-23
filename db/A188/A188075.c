/*
 * A188075  Positions of 0 in the zero-one sequence [n*r]-[4*r]-[n*r-4*r],
 *          where r=sqrt(3), n>=1.
 * https://oeis.org/A188075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A188075
 *
 *  a(n) = the n-th positive integer k (k≥4) such that
 *         floor(k*sqrt(3)) - floor((k-4)*sqrt(3)) = 6.
 *
 *  The sequence is obtained by scanning the integer positions k
 *  and recording those that satisfy the difference condition.
 *
 *  Implementation details:
 *  • The integer part of k*sqrt(3) is computed exactly as
 *    floor(sqrt(3*k^2)) using an integer square‑root routine.
 *  • The implementation keeps a static cache of the zero positions,
 *    growing it on demand so that any order of calls to a(n) is
 *    supported.
 *
 *  All arithmetic is performed with the integer type seq_t,
 *  and no floating point or external libraries are used.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* ---------- Integer square‑root for arbitrary seq_t ---------- */
static seq_t
isqrt_seq(seq_t x)
{
    if (x <= 0)
        return 0;

    unsigned bl = bitlen_seq_t(x);   /* number of bits in x */
    int shift = (bl / 2) + 1;        /* high bit of the trial root   */
    seq_t  bit   = (seq_t)1 << shift;
    seq_t  root  = 0;

    while (bit != 0) {
        seq_t mid = root + bit;
        if (mid * mid <= x)
            root = mid;
        bit >>= 1;
    }
    return root;
}

/* ---------- floor(k*sqrt(3)) --------------------------------- */
static seq_t
floor_sqrt3(idx_t k)
{
    seq_t k_seq = (seq_t)k;
    seq_t prod  = k_seq * k_seq;   /* k^2          */
    prod        = prod * (seq_t)3; /* 3*k^2        */
    return isqrt_seq(prod);       /* floor(sqrt(3*k^2)) */
}

/* ---------- Cache for zero positions --------------------------- */
static seq_t *cache = NULL;
static size_t cap   = 0;
static size_t len   = 0;   /* number of entries stored  */
static idx_t  lastk = 0;   /* largest position scanned  */

/* Grow cache array at least to newcap elements */
static void
grow_cache(size_t newcap)
{
    if (newcap <= cap)
        return;

    size_t newsize = cap ? cap * 2 : 16;
    while (newsize < newcap)
        newsize <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    cache = tmp;
    cap   = newsize;
}

/* Ensure that at least n zero positions are stored in the cache */
static void
ensure_terms(idx_t n)
{
    if (len >= n)
        return;

    for (idx_t k = lastk + 1; ; ++k) {
        /* Only positions k ≥ 4 can satisfy the condition. */
        if (k >= 4) {
            seq_t fk   = floor_sqrt3(k);
            seq_t fk4  = floor_sqrt3(k - 4);
            if (fk - fk4 == 6) {                 /* zero position found */
                grow_cache(len + 1);
                cache[len++] = k;
            }
        }
        if (len >= n) {
            lastk = k;
            break;
        }
    }
}

/* -------------- Public routine -------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)              /* the sequence is 1‑based; 0 is undefined */
        return 0;

    ensure_terms(n);
    return cache[n - 1];
}
