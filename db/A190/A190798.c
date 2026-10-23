/*
 * A190798  Maximum value of k^2 * (n-k).
 * https://oeis.org/A190798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k^2 * (n - k) where k = round(2*n/3). Equivalent
 *            piecewise definitions: n%3 == 0 : a = 4 * (n/3)^3 n%3 == 1 : a =
 *            (n/3) * (2*(n/3)+1)^2 n%3 == 2 : let m = (n/3)+1; a = m *
 *            (2*m-1)^2 This follows directly from the OEIS entry
 * Range:     u8 n <= 11, u16 n <= 76, u32 n <= 3071, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n / 3;
    idx_t r = n % 3;
    seq_t ans;

    switch (r) {
        case 0: {
            seq_t mm = (seq_t)m;
            ans = mm * mm * mm;
            ans *= (seq_t)4;
            break;
        }
        case 1: {
            seq_t mm = (seq_t)m;
            seq_t t  = (seq_t)(2 * m + 1);
            ans = mm * t * t;
            break;
        }
        case 2: {
            idx_t mp = m + 1;          /* m + 1 = (n/3)+1 = (n+1)/3 */
            seq_t mm = (seq_t)mp;
            seq_t t  = (seq_t)(2 * mp - 1);
            ans = mm * t * t;
            break;
        }
    }

    return ans;
}
