/*
 * A143110  Triangle read by rows, A051731 * A000034 * 0^(n-k), 1<=k<=n.
 * https://oeis.org/A143110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows:
   T(n,k) = 2 if k is even and divides n,
            1 if k is odd and divides n,
            0 otherwise.
   a(n) is the row-major concatenation of the triangle T. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* offset is 1, but defensive */
    /* Find the row r such that r*(r+1)/2 >= n  */
    idx_t r = 1;
    idx_t sum = 1;                 /* trianglesizes 1,3,6,10,... */
    while (sum < n) {
        ++r;
        sum += r;
    }
    /* r is now the row.  sum is the cumulative total up to row r.
       The previous cumulative total is sum - r. */
    idx_t k = n - (sum - r);       /* column index within the row */
    seq_t val = 0;
    if (r % k == 0) {              /* k divides r? */
        if (k % 2 == 0)
            val = 2;
        else
            val = 1;
    }
    return val;
}
