/*
 * A079647  Integer part of the cube root of n and integer part of the square
 *          root of n both divide n.
 * https://oeis.org/A079647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   int_cube_root_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS A079647.
   An integer n is included iff floor(sqrt(n)) and floor(cuberoot(n))
   both divide n. This function searches forward, memoising the
   sequence so that calls can be in any order. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/int_cube_root_u64.h"

/* Integer square root for a 64‑bit unsigned value, returned as seq_t. */
static inline seq_t int_sqrt_u64(idx_t x)
{
    idx_t low = 0;
    idx_t high = (x > ((idx_t)1 << 32) ? ((idx_t)1 << 32) : x);
    idx_t mid, res = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        idx_t sq = mid * mid;          /* mid <= 2^32 ⇒ no overflow */
        if (sq <= x) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* stored sequence values    */
    static size_t cap   = 0;        /* allocated capacity        */
    static size_t cnt   = 0;        /* number of values stored   */
    static idx_t  cand  = 1;        /* next integer to test      */

    while (cnt < n) {                /* generate until we have n terms */
        seq_t val = (seq_t)cand;
        seq_t r   = int_sqrt_u64(cand);
        seq_t c   = int_cube_root_u64(cand);

        if (c != 0 && r != 0 && val % r == 0 && val % c == 0) {
            /* store the value */
            if (cnt >= cap) {
                size_t new_cap = cap ? cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort(); /* out of memory */
                cache = tmp;
                cap   = new_cap;
            }
            cache[cnt++] = val;
        }
        cand++; /* next integer to test */
    }
    return cache[n - 1];
}
