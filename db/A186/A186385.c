/*
 * A186385  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=5i and g(j)=j(j+1)/2 (triangular
 *          number). Complement of A186386.
 * https://oeis.org/A186385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊(⌊√(40*n + 1)⌋ – 1)/2⌋
 * Range:     u8 n <= 6, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the rank of 5*n in the sorted merge of 5*i and j(j+1)/2,
   where ties are resolved by putting the triangular number before 5*i. */

#include <stddef.h>                 /* for size_t */
#include "../utils/bitlen_seq_t.h"

/* Integer square‑root floor for seq_t */
static seq_t isqrt_floor(seq_t x)
{
    if (x == (seq_t)0) {
        return (seq_t)0;
    }
    size_t bits = bitlen_seq_t(x);
    seq_t low  = (seq_t)0;
    seq_t high = (seq_t)1 << ((bits + 1) / 2);   /* 2^(ceil(bits/2)) >= sqrt(x) */

    /* binary search for floor(sqrt(x)) */
    while (low < high) {
        seq_t mid = (low + high + (seq_t)1) >> 1;     /* upper mid */
        /* mid*mid <= x  <=>  mid <= x/mid  (avoid overflow) */
        if (mid <= x / mid) {
            low = mid;
        } else {
            high = mid - (seq_t)1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Sequence starts at n = 1 */
    seq_t N = (seq_t)n;                       /* 5*n is a 5*i member */
    seq_t S = (seq_t)40 * N + (seq_t)1;       /* 40*n + 1 == 8*(5*n)+1 */
    seq_t sqrtS = isqrt_floor(S);             /* floor(sqrt(40*n+1)) */
    seq_t t = (sqrtS - (seq_t)1) / (seq_t)2;  /* floor((sqrtS-1)/2) */
    return N + t;                             /* rank of 5*n in the merged list */
}
