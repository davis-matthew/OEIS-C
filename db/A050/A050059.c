/*
 * A050059  a(n)=a(n-1)+a(m), where m=2n-2-2^(p+1) and 2^p<n-1<=2^(p+1), for n
 *          >= 4.
 * https://oeis.org/A050059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=a(n-1)+a(m), m=2n-2-2^(p+1), with 2^p<n-1<=2^(p+1); a(1)=1,
 *            a(2)=3, a(3)=1
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 126, u64 n <= 505,
 *            u128 n <= 3083, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

static seq_t *seq = NULL;       /* array of computed values, seq[0] unused */
static size_t   cap  = 0;       /* current capacity (>= len+1) */
static size_t   len  = 0;       /* number of terms actually stored (>=3 after init) */

static void ensure_capacity(size_t needed)
{
    if (cap >= needed)
        return;
    /* double capacity until sufficient, but at least needed */
    size_t newcap = cap ? cap << 1 : 16;
    while (newcap < needed)
        newcap <<= 1;
    seq = (seq_t *)realloc(seq, (newcap + 1) * sizeof(seq_t));
    cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* undefined, but safe */

    /* initialise base terms once */
    if (len == 0) {
        ensure_capacity(32);
        seq[1] = (seq_t)1;
        seq[2] = (seq_t)3;
        seq[3] = (seq_t)1;
        len = 3;
    }

    if (n <= len)
        return seq[n];

    /* compute missing terms up to n */
    for (size_t i = len + 1; i <= (size_t)n; ++i)
    {
        ensure_capacity(i);
        if (i == 1)
            seq[i] = (seq_t)1;
        else if (i == 2)
            seq[i] = (seq_t)3;
        else if (i == 3)
            seq[i] = (seq_t)1;
        else
        {
            /* recurrence for n >= 4 */
            unsigned p = floor_log2_uint64((idx_t)i - 2);          /* 2^p < i-1 <= 2^(p+1) */
            idx_t power = ((idx_t)1) << (p + 1);                  /* 2^(p+1) */
            idx_t m = ((idx_t)2 * i) - 2 - power;                 /* 2i-2-2^(p+1) */
            seq[i] = seq[i - 1] + seq[m];
        }
    }
    len = (size_t)n;
    return seq[n];
}
