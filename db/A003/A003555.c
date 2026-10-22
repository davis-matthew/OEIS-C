/*
 * A003555  Sum_{i=1..(10^n - 1)/9} i, or ((10^n -1)/9)*((10^n -1)/9 +1)/2
 *          (n-th term is the middle 2(n-1) digits of the (n+9)-th term for n
 *          > 1).
 * https://oeis.org/A003555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = R*(R+1)/2 where R = A002275(n) = (10^n - 1)/9
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t r = 0;
    idx_t i;
    for (i = 0; i < n; i++)
        r = r * 10 + 1;
    return r * ((r + 1) / 2);
}
