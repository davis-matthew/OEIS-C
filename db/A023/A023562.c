/*
 * A023562  Convolution of A023531 and odd numbers.
 * https://oeis.org/A023562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^n A023531(k) * (2n‑2k+1) * where A023531(k)=1 iff
 *            8k+9 is a perfect square, otherwise 0. * Recurrence: a(1)=0,
 *            a(n)=a(n‑1)+2·Σ_{k=1}^{n‑1}A023531(k)+A023531(n)
 * Range:     u8 n <= 33, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include "../utils/mem.h"

/* Check whether 8*n+9 is a perfect square.  */
static inline bool
is_8n9_square (idx_t n)
{
    __int128 t = (__int128)8 * n + 9;   /* 8n+9 fits in 128 bits.  */
    long double d = sqrt ( (long double) t );
    uint64_t m = (uint64_t) d;
    if ( (__int128)m * m > t )
        while ( (__int128)m * m > t ) --m;
    else
        while ( (__int128)(m + 1) * (m + 1) <= t ) ++m;
    return ( (__int128)m * m == t );
}

/* Cached sequence data.  */
static seq_t *cached  = NULL;        /* array of a(n), index 1..seq_len.  */
static idx_t  seq_len = 0;           /* maximum index stored.  */
static idx_t  seq_cap = 0;           /* allocated capacity.   */
static idx_t  cum_cnt = 0;           /* Σ_{k=1}^{current} A023531(k).  */
static seq_t  last_val = 0;          /* a(seq_len).  */

/* Ensure the cache contains a(n) for all n up to 'need'.  */
static void
ensure_up_to (idx_t need)
{
    if (need <= seq_len)
        return;

    /* Allocate or grow the cache.  */
    if (cached == NULL) {
        seq_cap = 16;
        cached = mem_new(seq_t, (seq_cap + 1));
        cached[0] = 0;             /* unused dummy to make indices 1.. */
        seq_len = 0;
        cum_cnt = 0;
        last_val = 0;
    }
    else if (need > seq_cap) {
        while (seq_cap < need)
            seq_cap <<= 1;
        cached = mem_resize(seq_t, cached, (seq_cap + 1));
    }

    for (idx_t i = seq_len + 1; i <= need; ++i) {
        bool cn = is_8n9_square (i);
        __int128 prod = (__int128)cum_cnt * 2 + (cn ? 1 : 0);
        seq_t diff = (seq_t)prod;
        last_val = last_val + diff;
        cached[i] = last_val;
        if (cn)
            ++cum_cnt;
    }
    seq_len = need;
}

/* Main function requested by the problem.  */
seq_t
a (idx_t n)
{
    if (n == 0)          /* unsupported for this offset; return 0.  */
        return 0;
    ensure_up_to (n);
    return cached[n];
}
