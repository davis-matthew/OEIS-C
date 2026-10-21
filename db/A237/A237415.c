/*
 * A237415  For k in {2,3,...,9} define a sequence as follows: a(0)=0; for
 *          n>=0, a(n+1)=a(n)+1, unless a(n) ends in k, in which case a(n+1)
 *          is obtained by replacing the last digit of a(n) with the digit(s)
 *          of k^3. This is k(2).
 * https://oeis.org/A237415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + a(n-5) - a(n-6), a(0)=0, a(1)=1, a(2)=2, a(3)=8,
 *            a(4)=9, a(5)=10
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values */
    static const seq_t init[6] = {0, 1, 2, 8, 9, 10};

    if (n < 6)
        return init[n];

    seq_t buf[6];
    for (idx_t i = 0; i < 6; ++i)
        buf[i] = init[i];

    for (idx_t i = 6; i <= n; ++i) {
        idx_t idx  = i % 6;
        idx_t im1  = (i - 1) % 6;
        idx_t im5  = (i - 5) % 6;
        idx_t im6  = (i - 6) % 6;
        buf[idx] = buf[im1] + buf[im5] - buf[im6];
    }

    return buf[n % 6];
}
