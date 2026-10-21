/*
 * A209304  Table T(n,k)=n+4*k-4 n, k > 0, read by antidiagonals.
 * https://oeis.org/A209304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 4*(t+1) - 3*(n - t*(t+1)/2) where t = max{ k | k*(k+1)/2
 *            < n } (i.e. the diagonal index)
 * Range:     u8 n <= 2080, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses integer arithmetic only; no floating point or external helpers. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, defensive */

    /* Find the largest t with t*(t+1)/2 < n.  Use binary search */
    idx_t low  = 0;
    idx_t high = 1;
    /* Expand high until the condition fails */
    while ( ( (__uint128_t)high * (high + 1) / 2 ) < n ) {
        high <<= 1;                       /* double high */
        if (high == 0) {                  /* overflow guard */
            high = n;
            break;
        }
    }
    /* Binary–search the exact t */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;                 /* upper mid */
        if ( ( (__uint128_t)mid * (mid + 1) / 2 ) < n )
            low = mid;
        else
            high = mid - 1;
    }
    idx_t t   = low;                                     /* diagonal index */
    seq_t tri = (seq_t)( ( (__uint128_t)t * (t + 1) ) / 2 ); /* t*(t+1)/2 */

    seq_t diff = (seq_t)( n - tri );                     /* n - tri  (positive) */
    seq_t res  = (seq_t)(4 * (t + 1)) - (seq_t)(3 * diff);
    return res;
}
