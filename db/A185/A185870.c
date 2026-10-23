/*
 * A185870  (Even,odd)-polka dot array in the natural number array A000027, by
 *          antidiagonals.
 * https://oeis.org/A185870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is obtained by reading the array T(n,k) along anti‑diagonals,
 * where T(n,k) = 2*n + (n+k-1)*(2*n+2*k-3)  for n,k ≥ 1.
 * The sequence starts with a(1)=T(1,1)=3.
 */

#include <stdint.h>

/* integer square–root for 64‑bit unsigned values, floor(sqrt(x)) */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = x < 0xFFFFFFFFULL ? x : 0xFFFFFFFFULL; /* hi ≤ 2^32-1 */
    idx_t r = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid * mid <= x) {
            r = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* behaviour below the offset is undefined */

    /* find diagonal index s such that s*(s-1)/2 ≥ n */
    idx_t t   = 1 + 8 * n;                         /* 1 + 8n */
    idx_t root = isqrt_u64(t);                     /* floor sqrt(1+8n) */
    idx_t s    = (1 + root) / 2;                   /* initial estimate */

    /* correct s if the estimate was too small */
    seq_t sum = ((seq_t)s * (s - 1)) / 2;
    while ((seq_t)n > sum) {
        ++s;
        sum = ((seq_t)s * (s - 1)) / 2;
    }

    /* offset within this diagonal (0‑based) */
    seq_t prev_sum = ((seq_t)(s - 1) * (s - 2)) / 2;
    idx_t offset   = (idx_t)( (seq_t)n - prev_sum - 1 );   /* 0 ≤ offset < s-1 */

    idx_t i = offset + 1;          /* row index */
    idx_t j = s - i;               /* column index (j ≥ 1) */

    /* compute T(i,j) */
    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;
    seq_t val = 2 * ii + (ii + jj - 1) * (2 * ii + 2 * jj - 3);
    return val;
}
