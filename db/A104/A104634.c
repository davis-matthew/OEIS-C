/*
 * A104634  Triangle T(n,k) = (k-1-n)*(k-2-n)*(k+2*n)/6, 1<=k<=n.
 * https://oeis.org/A104634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (row+1-col)*(row+2-col)*(col+2*row)/6 where row =
 *            smallest r such that r*(r+1)/2 >= n, col = n - (row-1)*row/2, 1
 *            ≤ col ≤ row
 * Range:     u8 n <= 36, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset is 1, return 0 for n==0 (unused but safe) */
    if (n == 0)
        return 0;

    /* find row r: the smallest r with r*(r+1)/2 >= n */
    idx_t low  = 1;
    idx_t high = n;                   /* n is at most total number of terms */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1);
        idx_t tri = (idx_t)(prod >> 1);    /* (mid*(mid+1))/2 */
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* number of terms before row r */
    unsigned __int128 prod_prev = (unsigned __int128)(r - 1) * r;
    idx_t prev_tri = (idx_t)(prod_prev >> 1);    /* (r-1)*r/2 */
    idx_t col = n - prev_tri;   /* 1 ≤ col ≤ r */

    /* compute the positive factors of the formula */
    seq_t f1 = (seq_t)(r + 1 - col);          /* (r+1-col) */
    seq_t f2 = (seq_t)(r + 2 - col);          /* (r+2-col) */
    seq_t f3 = (seq_t)(col + 2 * (seq_t)r);   /* (col+2*r) */

    /* divide by 2: one of f1 or f2 is even */
    if ((f1 & 1) == 0)
        f1 >>= 1;
    else
        f2 >>= 1;

    /* divide by 3: at least one of f1, f2, f3 is divisible by 3 now */
    if ((f1 % 3) == 0)
        f1 /= 3;
    else if ((f2 % 3) == 0)
        f2 /= 3;
    else
        f3 /= 3;

    /* final product (value fits in seq_t for the intended ranges) */
    seq_t result = f1 * f2;
    result = result * f3;
    return result;
}
