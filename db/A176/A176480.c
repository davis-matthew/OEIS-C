/*
 * A176480  Triangle: let b(n) = 3*b(n - 1) - 2*b(n - 2) + b(n - 3), then
 *          T(n,m) = b(n) - b(m) - b(n - m) + 1.
 * https://oeis.org/A176480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,m) where r = floor((sqrt(8*n+1)-1)/2) m = n -
 *            r*(r+1)/2 T(r,m) = b(r) - b(m) - b(r-m) + 1 and b is defined by
 *            b(0)=0, b(1)=1, b(2)=3, b(k)=3*b(k-1)-2*b(k-2)+b(k-3) for k>=3
 * Range:     u8 n <= 36, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

/* we assume idx_t and seq_t are defined by the build system */
seq_t a(idx_t n)
{
    /* compute row index r */
    __int128 tn = (__int128)8 * n + 1;      /* 8*n+1   (may overflow 64-bit) */
    long double fd = sqrtl((long double)tn);
    idx_t s = (idx_t)fd;
    /* adjust s to be floor(sqrt(tn)) */
    while (( (__int128)(s+1)*(s+1) ) <= tn) s++;
    while ( (__int128)s*s > tn ) s--;
    idx_t r = (s - 1) / 2;                 /* row number (0‑based) */

    /* compute column m */
    idx_t m_offset = r * (r + 1) / 2;       /* terms before row r */
    idx_t m = n - m_offset;                /* column inside the row */

    /* static cache of b values */
    static seq_t *b_cache = NULL;
    static size_t b_cap = 0;
    static size_t b_len = 0;               /* number of entries computed */

    if (b_cache == NULL) {                 /* first call: initialise */
        b_cap = 3;
        b_cache = (seq_t *)malloc(b_cap * sizeof(seq_t));
        if (!b_cache) return 0;            /* should not happen */
        b_cache[0] = (seq_t)0;
        b_cache[1] = (seq_t)1;
        b_cache[2] = (seq_t)3;
        b_len  = 3;
    }

    /* grow cache if needed */
    if ((size_t)r + 1 > b_cap) {
        size_t new_cap = (size_t)r + 1;
        seq_t *new_cache = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;          /* should not happen */
        b_cache = new_cache;
        b_cap   = new_cap;
    }

    /* compute missing b values up to r */
    for (size_t k = b_len; k <= (size_t)r; ++k) {
        /* recurrence: b[k] = 3*b[k-1] - 2*b[k-2] + b[k-3] */
        seq_t val = 3 * b_cache[k - 1] - 2 * b_cache[k - 2] + b_cache[k - 3];
        b_cache[k] = val;
    }
    if (b_len <= (size_t)r) b_len = (size_t)r + 1;

    /* compute the triangle entry */
    seq_t ans = b_cache[r] - b_cache[m] - b_cache[r - m] + (seq_t)1;
    return ans;
}
