/*
 * A053459  Open disk numbers (version 4): a(n) is the number of points
 *          (i+j/2,j*sqrt(3)/2), i,j integers (triangular grid) contained in
 *          an open disk of diameter n, centered at (1/2,0).
 * https://oeis.org/A053459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) counts points (i+j/2 , j*sqrt(3)/2) in the open disk of *
 *            diameter n centered at (1/2,0). Equivalent condition: * v = 2i +
 *            j - 1, v^2 + 3*j^2 < n^2 with v ≡ j-1 (mod 2). The *
 *            implementation iterates over j ≥ 0, counts admissible v for each
 *            j, * and mirrors j > 0 to negative j. All arithmeti...
 * Range:     u8 n <= 5, u16 n <= 34, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* For types used in seq_isqrt only */

/* Integer square root via binary search: floor(sqrt(x)) for seq_t. */
static seq_t seq_isqrt(seq_t x)
{
    seq_t low = 0, high = x;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        seq_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            if (mid == high) break;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t n2 = (seq_t)n * (seq_t)n;            /* n^2 */

    for (idx_t j = 0; ; ++j) {
        idx_t  j2 = j * j;                     /* j^2  (safe for typical n) */
        seq_t  t  = (seq_t)3 * (seq_t)j2;       /* 3*j^2  */

        /* When 3*j^2 >= n^2 we have no further solutions. */
        if (t >= n2) break;

        seq_t m = n2 - t;                      /* m = n^2 - 3*j^2  (positive) */
        /* Count integer v with  v^2 < m  and  v ≡ j-1 (mod 2). */
        seq_t L = seq_isqrt(m - 1);            /* floor(sqrt(m-1))  */

        /* Parity requirement: v parity must equal (j-1) mod 2. */
        int parity = (j & 1) ? 0 : 1;          /* 1 for even j, 0 for odd j */

        /* Number of integers in [-L,L] of the required parity. */
        idx_t cnt;
        if ((L & 1) == 0) {                   /* L even */
            cnt = (parity == 0) ? (idx_t)(L + 1) : (idx_t)L;
        } else {                              /* L odd */
            cnt = (parity == 1) ? (idx_t)(L + 1) : (idx_t)L;
        }

        if (j == 0)
            total += (seq_t)cnt;               /* j = 0 only once */
        else
            total += 2 * (seq_t)cnt;           /* mirror j < 0 */
    }
    return total;
}
