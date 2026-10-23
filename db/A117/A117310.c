/*
 * A117310  Triangular numbers for which the product of the digits is a
 *          hexagonal number.
 * https://oeis.org/A117310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 47, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search a(n) by testing triangular numbers whose digit product is
   a hexagonal number. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_product.h"
#include "../utils/floor_sqrt.h"

#ifdef __cplusplus
#include <vector>
#endif

/* Check whether x is a hexagonal number: x = m(2m‑1) for some m ≥ 0. */
static bool is_hexagonal(seq_t x)
{
    if (x == 0)               /* H_0 = 0 */
        return true;
    seq_t d = 1 + (seq_t)8 * x;      /* discriminant 1 + 8*x */
    seq_t s = floor_sqrt(d);
    if (s * s != d)
        return false;
    return ((seq_t)1 + s) % 4 == 0;
}

/* Return the nth term of A117310 (0‑based index). */
seq_t a(idx_t n)
{
#ifndef __cplusplus
    static seq_t *cache = NULL;      /* stored qualifying triangular numbers */
    static size_t cap = 0;           /* allocated capacity of cache      */
    static idx_t count = 0;          /* number of terms stored in cache   */
    static idx_t cur_k = 0;          /* current index k of triangular number */
    static seq_t cur_tri = 0;        /* current triangular number T_k */

    /* Generate terms until we have at least (n+1) stored. */
    while (count <= n) {
        /* Produce next triangular number */
        if (count == 0) {           /* first term T_0 = 0 */
            cur_k   = 0;
            cur_tri = 0;
        } else {
            ++cur_k;
            cur_tri += cur_k;      /* T_k = T_{k‑1} + k */
        }

        /* Test if digit product is hexagonal */
        seq_t prod = digit_product(cur_tri);
        if (is_hexagonal(prod)) {
            /* Ensure cache has space for new term */
            if ((size_t)count >= cap) {
                size_t newcap = cap == 0 ? 1 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();   /* allocation failure */
                cache = tmp;
                cap = newcap;
            }
            cache[count] = cur_tri;
            ++count;
        }
    }

    return cache[n];
#else
    static std::vector<seq_t> cache;  /* stored qualifying triangular numbers */
    static idx_t cur_k = 0;           /* current index k of triangular number */
    static seq_t cur_tri = 0;         /* current triangular number T_k */
    static idx_t count = 0;           /* number of terms stored in cache   */

    /* Generate terms until we have at least (n+1) stored. */
    while (count <= n) {
        if (count == 0) {            /* first term T_0 = 0 */
            cur_k   = 0;
            cur_tri = 0;
        } else {
            ++cur_k;
            cur_tri += cur_k;        /* T_k = T_{k‑1} + k */
        }

        seq_t prod = digit_product(cur_tri);
        if (is_hexagonal(prod)) {
            cache.push_back(cur_tri);
            ++count;
        }
    }

    return cache[n];
#endif
}
