/*
 * A176482  Triangle, read by rows, defined by T(n, k) = b(n) - b(k) - b(n-k)
 *          + 1 (see formula section for recurrence for b(n)).
 * https://oeis.org/A176482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 66, u32 n <= 211, u64 n <= 780,
 *            u128 n <= 3003, big any n
 * Helpers:   _floor_isqrt_u64, seq_alloc, seq_alloc_array, seq_free,
 *            seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Triangle, read by rows, defined by T(n, k) = b(n) - b(k) - b(n-k) + 1  
 *  with b(n) = 4*b(n-1) - 3*b(n-2) + 2*b(n-3) - b(n-4), b(0)=0, b(1)=1, b(2)=4, b(3)=13.  
 *  The sequence a(n) is the triangle read by rows (a(0)=T(0,0), a(1)=T(1,0), …).  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_free.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.  */
    idx_t r;
    {
        /* Compute floor(sqrt(8*n+1)) */
        idx_t root = _floor_isqrt_u64((idx_t)8 * n + 1);
        r = (root - 1) / 2;
        /* correct for rounding errors */
        while ((r + 1) * (r + 2) / 2 <= n) r++;
        while (r * (r + 1) / 2 > n) r--;
    }
    idx_t k = n - r * (r + 1) / 2;
    /* Cache b values up to r.  */
    static seq_t *b_cache = NULL;
    static size_t b_cache_size = 0;
    static size_t b_cache_cap = 0;
    if (r >= (idx_t)b_cache_cap) {
        size_t new_cap = (size_t)r + 1;
        seq_t *new_cache = seq_alloc_array(new_cap);
        if (b_cache) {
            /* copy existing values */
            for (size_t i = 0; i < b_cache_size; i++)
                new_cache[i] = b_cache[i];
            seq_free_array(b_cache);
        }
        /* fill new entries */
        for (size_t i = b_cache_size; i < new_cap; i++) {
            if (i == 0) new_cache[0] = (seq_t)0;
            else if (i == 1) new_cache[1] = (seq_t)1;
            else if (i == 2) new_cache[2] = (seq_t)4;
            else if (i == 3) new_cache[3] = (seq_t)13;
            else {
                seq_t val = new_cache[i-1] << 2;          /* 4*b[i-1] */
                seq_t tmp = (seq_t)3 * new_cache[i-2];   /* 3*b[i-2] */
                val -= tmp;                              /* -3*b[i-2] */
                tmp = (seq_t)2 * new_cache[i-3];         /* 2*b[i-3] */
                val += tmp;                              /* +2*b[i-3] */
                val -= new_cache[i-4];                   /* -b[i-4] */
                new_cache[i] = val;
            }
        }
        b_cache = new_cache;
        b_cache_size = new_cap;
        b_cache_cap = new_cap;
    }

    seq_t val = b_cache[(size_t)r];
    val -= b_cache[(size_t)k];
    val -= b_cache[(size_t)(r - k)];
    val += (seq_t)1;
    return val;
}
