/*
 * A143311  Triangle read by rows, A127648 * A126988; 1<=k<=n.
 * https://oeis.org/A143311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) is the term of the triangle read by rows where 
 *           the entry at row r, column k (1 ≤ k ≤ r) is
 *              T(r,k) = r^2 / k   if  k divides r
 *                      = 0       otherwise.
 *           a(n) is obtained by reading the triangle by rows, i.e.  
 *           a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), a(4)=T(3,1), … 
 *           Row sums equal n·σ(n) where σ(n) is the sum of divisors.
 */
seq_t a(idx_t n)
{
    /* find row r such that offset(r) <= n < offset(r+1) */
    /* offset(r) = r*(r-1)/2 + 1  (first index of row r)   */
    idx_t low = 1, high = n;          /* n ≥ 1 by offset 1          */
    while (low < high)
    {
        idx_t mid = low + (high - low + 1) / 2;
        /* compute offset(mid) without overflow:  mid*(mid-1)/2 + 1  */
        idx_t prod;
        if (mid & 1)                /* mid odd  -> mid * ((mid-1)/2)       */
            prod = mid * ((mid - 1) / 2);
        else                        /* mid even -> (mid/2) * (mid-1)     */
            prod = (mid / 2) * (mid - 1);
        idx_t offset_mid = prod + 1;
        if (offset_mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    /* column index within the row */
    /* compute offset(r) again */
    idx_t prod_r;
    if (r & 1)
        prod_r = r * ((r - 1) / 2);
    else
        prod_r = (r / 2) * (r - 1);
    idx_t offset_r = prod_r + 1;
    idx_t k = n - offset_r + 1;

    /* return value */
    if (r % k == 0)
    {
        seq_t r_sq = (seq_t)r * (seq_t)r;
        return r_sq / k;
    }
    else
    {
        return (seq_t)0;
    }
}
