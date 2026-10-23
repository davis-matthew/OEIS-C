/*
 * A157632  Triangle T(n,m) read by rows: 1 in column m=0 and on the diagonal,
 *          else 3*n*m*(n-m).
 * https://oeis.org/A157632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 1 if n is the first or last element of its row in the
 *            triangle T with T(i,j)=1 (j=0 or j=i) and otherwise 3*i*j*(i-j),
 *            where i = floor( (sqrt(8*n+1)-1)/2 ) and j = n - i*(i+1)/2. This
 *            implements the OEIS A157632 sequence
 * Range:     u8 n <= 37, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
/* seq_t and idx_t types are defined by the build system. */

seq_t a(idx_t n)
{
    /* Find row i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2.
       Binary search on i.  The upper bound i <= n because
       a row with i elements has at least i+1 total indices. */
    idx_t low = 0, high = n, mid, i;
    while (low <= high) {
        /* Avoid overflow in mid calculation: use addition and shift. */
        mid = low + (high - low) / 2;
        /* Compute mid*(mid+1)/2 using 128-bit to avoid overflow. */
        __int128 prod = (__int128)mid * (mid + 1) / 2;
        if (prod <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    i = high;                     /* Last index satisfying the inequality. */
    idx_t before = (__int128)i * (i + 1) / 2;   /* Elements before row i */
    idx_t j = n - before;                 /* Column index within the row */

    /* Compute the triangle entry. */
    if (j == 0 || j == i)
        return (seq_t)1;

    /* Multiplication is done in seq_t.  Cast operands to seq_t to keep
       intermediate type correct.  For seq_t being an arbitrary-precision
       integer class, the multiplications are handled appropriately. */
    seq_t val = (seq_t)3 *
                ((seq_t)i * (seq_t)j * (seq_t)(i - j));
    return val;
}
