/*
 * A052795  a(n) = (6*n)!/(5*n+1)!.
 * https://oeis.org/A052795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (6*n)! / (5*n+1)! = Π_{k=5*n+2}^{6*n} k, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case n==0 (or n==1 where the product range is empty) */
    if (n <= 1)
        return (seq_t)1;

    /* Compute the product of integers from (5*n+2) to (6*n) inclusive.
       Since the range contains n-1 factors, the loop is O(n). */
    idx_t start = 5 * n + 2;
    idx_t end   = 6 * n;
    seq_t  result = (seq_t)1;
    for (idx_t k = start; k <= end; ++k)
        result *= (seq_t)k;   /* implicit cast handles seq_t types */

    return result;
}
