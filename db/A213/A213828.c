/*
 * A213828  Rectangular array: (row n) = b**c, where b(h) = 3*h-2, c(h) =
 *          3*n-4+3*h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = j/2 * ( (3*i + 3*j - 1)*(3*j - 1) - 3*(j + 1)*(2*j - 1) )
 *            where (i,j) are the row and column indices of the nth element of
 *            the two‑dimensional array obtained by reading it by falling
 *            antidiagonals. For (i,j) the order number n is n = (i + j -
 *            2)*(i + j - 1)/2 + i Conversely, from n we reco...
 * Range:     u8 n <= 10, u16 n <= 253, u32 n <= 253, u64 n <= 253,
 *            u128 n <= 253, big any n
 * Helpers:   get_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_s.h"

static inline idx_t isqrt_u64(idx_t n)
/* floor(sqrt(n)) for unsigned 64‑bit n */
{
    if (n < 2) return n;
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;      /* upper mid to avoid endless loop */
        if ((__int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
/* main routine: return the nth term of OEIS A213828 */
{
    if (n == 0) return 0;                    /* not used – offset is 1 */

    /* Step 1: recover coordinates (i, j) from n */
    idx_t s = get_s(n);                       /* i + j = s */
    idx_t prev = (idx_t)(((__int128)(s - 2) * (s - 1)) / 2); /* cells before diag s */
    idx_t i = n - prev;                      /* row index (1‑based) */
    idx_t j = s - i;                          /* column index (1‑based) */

    /* Step 2: compute T(i,j) = convolution value */
    /* Use formula derived from (3h-2)*(3i+3j-3h-1) */
    const seq_t one  = (seq_t)1;
    const seq_t two  = (seq_t)2;
    const seq_t three= (seq_t)3;

    seq_t i_seq = (seq_t)i;
    seq_t j_seq = (seq_t)j;

    seq_t A     = three * i_seq + three * j_seq - one;
    seq_t term1 = A * (three * j_seq - one);
    seq_t term2 = three * (j_seq + one) * ((two * j_seq) - one);
    seq_t diff  = term1 - term2;
    seq_t num   = j_seq * diff;
    return num / two;                         /* exact division by 2 */
}
