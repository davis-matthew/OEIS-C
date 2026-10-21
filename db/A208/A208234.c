/*
 * A208234  Second inverse function (numbers of columns) for pairing function
 *          A188568.
 * https://oeis.org/A208234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Let (i,j) be the anti‑diagonal coordinates of n in the triangle
 *            A188568 (OEIS A188568). Then a(n) is a = i if i<j and j is odd a
 *            = j if i<j and j is even a = j if i>=j and i is even a = i if
 *            i>=j and i is odd. This follows from the piecewise definition
 *            given in the OEIS entry, simplified to avoid...
 * Range:     u8 n <= 32, u16 n <= 8937, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t */
    seq_t sn = n;

    /* compute s = floor(sqrt(8*n-7)) without floating point */
    seq_t val = 8 * sn - 7U;
    seq_t low = 0, high = val, mid, ans = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        if (mid == 0) {
            low = 1;
            ans = 0;
            continue;
        }
        if (mid <= val / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    seq_t s = ans;

    /* t = floor((s-1)/2) */
    seq_t t = (s - 1U) >> 1;

    /* i = n - t*(t+1)/2 */
    seq_t triangle = t * (t + 1U) / 2U;
    seq_t i = sn - triangle;

    /* j = (t*t + 3*t + 4)/2 - n */
    seq_t j = (t * t + 3U * t + 4U) / 2U - sn;

    /* decide answer using parity */
    if (i >= j) {
        if ((i & 1U) == 0U) /* i even */
            return j;
        else                /* i odd  */
            return i;
    } else {
        if ((j & 1U) == 0U) /* j even */
            return j;
        else                /* j odd  */
            return i;
    }
}
