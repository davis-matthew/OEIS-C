/*
 * A209301  Table T(n,k) n, k > 0, T(n,k)=n-k+1, if n>=k, T(n,k)=k-n+2, if n <
 *          k. Table read by sides of squares from T(1,n) to T(n,n), then from
 *          T(n,n) to T(n,1).
 * https://oeis.org/A209301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value read from the perimeter of the square of size
 *            t = ceil(sqrt(n)) in table T(i,j) defined by T(i,j) = i-j+1 if
 *            i>=j, else T(i,j) = j-i+2. The index n belongs to the k-th
 *            perimeter cell where k = n-(t-1)² (1 ≤ k ≤ 2t-1). See OEIS
 *            A209301 for details
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for 0 ≤ n ≤ 2^64-1 */
static idx_t isqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    /* Determine square size t = ceil(sqrt(n)) */
    idx_t t = isqrt_u64(n - 1) + 1;      /* n >= 1 */
    idx_t prev_sq = (t - 1) * (t - 1);   /* (t-1)², fits in 64‑bit */
    idx_t p = n - prev_sq;              /* position within the perimeter (1..2t-1) */

    idx_t i, j;
    if (p <= t) {          /* top side (from (1,t) to (t,t)) */
        i = p;             /* row */
        j = t;             /* column */
    } else {               /* bottom side (from (t,t-1) down to (t,1)) */
        i = t;
        j = 2 * t - p;    /* column */
    }

    if (i >= j)
        return (seq_t)(i - j + 1);
    else
        return (seq_t)(j - i + 2);
}
