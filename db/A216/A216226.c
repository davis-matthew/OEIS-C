/*
 * A216226  Square array T, read by antidiagonals: T(n,k) = 0 if n-k>=1 or if
 *          k-n>=4, T(0,0) = T(0,1) = T(0,2) = T(0,3) = 1, T(n,k) = T(n-1,k) +
 *          T(n,k-1).
 * https://oeis.org/A216226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A216226:
   Square array T read by antidiagonals.  
   T(0,k)=1 for 0≤k≤3, else 0;  
   For n≥1 and |k−n|≤3:  T(n,k)=T(n−1,k)+T(n,k−1), otherwise 0.
   a(n) = T(i,j) where i+j is the diagonal index d such that
          d*(d+1)/2 ≤ n < (d+1)*(d+2)/2,  i = n − d*(d+1)/2,
          j = d − i.                                             */

#include <stdint.h>

/* Fast doubling Fibonacci: returns (F(n),F(n+1)) in *fn and *fn1. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1) {
    if (n == 0) { *fn = 0; *fn1 = 1; return; }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);                   /* a = F(k), b = F(k+1)  */
    seq_t c = a * ((b << 1) - a);               /* F(2k) */
    seq_t d = a * a + b * b;                    /* F(2k+1) */
    if (n & 1) {                                /* odd */
        *fn  = d;
        *fn1 = c + d;
    } else {                                    /* even */
        *fn  = c;
        *fn1 = d;
    }
}

seq_t a(idx_t n) {
    /* Find the diagonal d: d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1u) / 2;
        if (t <= n) low = mid; else high = mid - 1;
    }
    idx_t d = low;
    unsigned __int128 prev = (unsigned __int128)d * (d + 1u) / 2;
    idx_t i = n - (idx_t)prev;         /* row index  (offset on the diagonal) */
    idx_t j = d - i;                   /* column index */

    /* Zero outside the allowed band */
    if (j < i || j > i + 3) return (seq_t)0;

    /* Special case i == 0: all non‑zero entries are 1 */
    if (i == 0) return (seq_t)1;

    /* Difference j-i determines which Fibonacci number */
    idx_t diff = j - i;                /* 0 ≤ diff ≤ 3 */
    idx_t m = 2 * i - 1;               /* base index for F(2i-1) */
    if (diff == 1) m += 2;             /* F(2i+1)  */
    else if (diff >= 2) m += 3;        /* F(2i+2)  */

    seq_t fn, fn1;
    fib_pair(m, &fn, &fn1);            /* fn = F(m)  */
    return fn;
}
