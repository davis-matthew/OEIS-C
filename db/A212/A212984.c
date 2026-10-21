/*
 * A212984  Number of (w,x,y) with all terms in {0..n} and 3w = x+y.
 * https://oeis.org/A212984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor(2*n/3) + floor(n^2/3)
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor(2*n/3) */
    seq_t t1 = (seq_t)(((unsigned __int128)n * (seq_t)2) / (seq_t)3);

    /* floor(n^2/3) using division to avoid overflow */
    unsigned __int128 q = (unsigned __int128)n / (unsigned __int128)3;
    unsigned __int128 r = (unsigned __int128)n % (unsigned __int128)3;
    unsigned __int128 t2 = 3 * q * q + 2 * q * r;
    if (r == (unsigned __int128)2)
        t2 += 1;
    seq_t t2_s = (seq_t)t2;

    return (seq_t)1 + t1 + t2_s;
}
