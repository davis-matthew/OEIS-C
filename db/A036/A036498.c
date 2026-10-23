/*
 * A036498  Numbers of the form m*(6*m-1) and m*(6*m+1), where m is an
 *          integer.
 * https://oeis.org/A036498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = 6*k*k - k Formula: a(2k+1) = 6*k*k + k where k =
 *            floor(n/2)
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n >> 1;                      /* k = floor(n/2)                */
    seq_t kk = (seq_t)k;                  /* convert to seq_t               */
    seq_t val = (seq_t)6 * kk * kk;        /* 6*k*k in seq_t arithmetic     */
    if (n & 1ULL)                         /* odd n  -> a(2k+1)              */
        return val + kk;                  /* 6*k*k + k                      */
    else                                   /* even n -> a(2k)                */
        return val - kk;                  /* 6*k*k - k                      */
}
