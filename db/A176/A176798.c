/*
 * A176798  Triangle read by rows: T(n,m)=1 + n*(2*m + 1 + n)/2, 0<=m<=n.
 * https://oeis.org/A176798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 + r*(2*m + 1 + r)/2, * where r = floor((sqrt(8*n + 1) -
 *            1)/2) * and m = n - r*(r + 1)/2
 * Range:     u8 n <= 62, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t high = 1;
    while (1) {
        unsigned __int128 prod = (unsigned __int128)high * (high + 1);
        if ((prod / 2 > n) || (high > n))
            break;
        high <<= 1;
        if (high == 0)           /* overflow of high */
            break;
    }
    idx_t low = high >> 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1);
        if (prod / 2 <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;

    idx_t t = r * (r + 1) / 2;   /* trianglesize of rows before r */
    idx_t m = n - t;             /* column inside row r */

    seq_t rr = (seq_t)r;
    seq_t mm = (seq_t)m;
    seq_t num = rr * (mm * 2 + 1 + rr);  /* rr * (2*m + 1 + rr) */
    seq_t ret = 1 + num / 2;
    return ret;
}
