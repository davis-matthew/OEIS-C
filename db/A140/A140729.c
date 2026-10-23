/*
 * A140729  Diagonal A(n,n) of array A(k,n) = Product of first n of k-gonal
 *          pyramidal numbers.
 * https://oeis.org/A140729
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Π_{j=1..n} (1/6)·j·(j+1)·((n-2)·j + (5-n))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for int64_t */

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;   /* a(0), a(1), a(2) are undefined in the OEIS data */

    seq_t result = 1;

    for (idx_t j = 1; j <= n; ++j)
    {
        /* The factor ((n-2)·j + (5-n)) is always positive for j≥1,
           but (5-n) is negative for n>5, so compute it in signed type. */
        int64_t factor = (int64_t)(n - 2) * (int64_t)j + (int64_t)(5 - n);

        seq_t term = j;
        term *= (j + 1);
        term *= (seq_t)factor;
        term /= 6;              /* exact division */

        result *= term;
    }

    return result;
}
