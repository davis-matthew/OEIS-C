/*
 * A118821  2-adic continued fraction of zero, where a(n) = 2 if n is odd,
 *          -A006519(n/2) otherwise.
 * https://oeis.org/A118821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cofr, sign
 * Formula:   not stated
 * Range:     i8 n <= 511, i16 n <= 9729, i32 n <= 9729, i64 n <= 9729,
 *            i128 n <= 9729, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 2-adic continued fraction of zero (OEIS A118821)
    a(n) =  2                     if n is odd
           -A006519(n/2)          if n is even
    where A006519(k) is the highest power of 2 dividing k  */

seq_t a(idx_t n)
{
    /* odd indices give the constant 2 */
    if (n & 1U)
        return (seq_t)2;

    /* even indices: -A006519(n/2) */
    seq_t m = (seq_t)(n >> 1);   /* n/2 as seq_t */
    seq_t power = 1;             /* holds 2^k */

    /* find the highest power of 2 that divides m */
    while ((m & 1) == 0) {
        m >>= 1;
        power <<= 1;
    }

    return -(power);
}
