/*
 * A213778  Rectangular array: (row n) = b**c, where b(h) = h, c(h) =
 *          1+[(n-1+h)/2], n>=1, h>=1, [ ] = floor, and ** = convolution.
 * https://oeis.org/A213778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 3240, u32 n <= 3240, u64 n <= 3240,
 *            u128 n <= 3240, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = read the rectangular array T defined by   T(r,c) =
   sum_{h=1}^{c} h * ceil((r + c - h + 1)/2)
   in order of falling antidiagonals: (1,1), (1,2),(2,1), (1,3),(2,2),(3,1), ...
   This function returns the n-th term of that enumeration. */
#include <stdint.h>
#include <stddef.h>

/* external types provided by the build system */
extern seq_t a(idx_t);

seq_t a(idx_t N)
{
    if (N == 0) return 0;          /* undefined for n=0, but return 0 */

    /* find diagonal index d (0‑based) such that
       cumulative terms up to diagonal d >= N.
       Cumulative terms up to diagonal d = (d+1)*(d+2)/2.          */
    idx_t low = 0, high = (1ULL << 32);          /* high is safely > sqrt(2*N) for all N <= 2^64-1 */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 cum = (__int128)(mid + 1) * (mid + 2) / 2;
        if (cum >= N) high = mid;
        else          low  = mid + 1;
    }
    idx_t d = low;                       /* diagonal index */

    /* previous cumulative = d*(d+1)/2 */
    idx_t prev_cum = (idx_t)((__int128)d * (d + 1) / 2);
    idx_t offset = N - prev_cum - 1;      /* 0‑based offset inside diagonal */

    /* coordinates in the array: row = n, column = k */
    idx_t n = offset + 1;                /* row index (starts at 1) */
    idx_t k = (d + 1) - offset;          /* column index  (starts at 1) */

    /* compute T(n,k) = sum_{h=1}^{k} h * ceil((n + k - h + 1)/2) */
    seq_t sum = 0;
    for (idx_t h = 1; h <= k; ++h) {
        idx_t j = k - h + 1;             /* j = n + k - h + 1 minus n => j = k - h + 1 */
        idx_t val = n + j;               /* n + (k - h + 1) */
        seq_t c = (seq_t)(val + 1) / 2;  /* ceil(val/2) = (val+1)/2 */
        seq_t prod = (seq_t)h * c;
        sum += prod;
    }
    return sum;
}
