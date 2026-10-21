/*
 * A219087  a(n) = floor((n + 1/2)^(4/3)).
 * https://oeis.org/A219087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + 1/2)^(4/3))
 * Range:     u8 n <= 63, u16 n <= 4095, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* t = 2*n + 1  (exact integer representation of 2n+1) */
    idx_t t = n * 2 + 1;

    /* t4 = t^4 = (2n+1)^4, fits in idx_t for the input range */
    idx_t t2 = t * t;
    idx_t t4 = t2 * t2;

    /* Binary search for the largest k such that 16*k^3 <= t^4 */
    idx_t low  = 0;
    idx_t high = 1;

    /* Find an upper bound for k */
    while (1) {
        idx_t h3 = high * high * high;
        if (h3 > t4 / 16)
            break;
        high <<= 1;
    }

    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) / 2;
        idx_t m3  = mid * mid * mid;

        /* Compare 16*mid^3 with t^4 */
        if (m3 > t4 / 16) {
            if (mid == 0)
                break;
            high = mid - 1;
        } else {
            ans = mid;
            low = mid + 1;
        }
    }

    return (seq_t)ans;
}
