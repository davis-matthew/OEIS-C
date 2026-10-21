/*
 * A203233  (n-1)-st elementary symmetric function of the first n terms of the
 *          periodic sequence (3,2,3,2,3,2,...).
 * https://oeis.org/A203233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for even n=2k: a(n)=5*k*6^(k-1); for odd n=2k+1:
 *            a(n)=(5*k+2)*6^k/2;
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 46, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset */
    if (n == 1) return (seq_t)1;

    /* write n as 2k (+1 for odd case) */
    idx_t k = n / 2;

    /* compute powers of 6 */
    seq_t pow = (seq_t)1;
    if (n % 2 == 0) {              /* even case: need 6^(k-1) */
        for (idx_t i = 1; i < k; ++i)
            pow *= (seq_t)6;
        /* result = 5*k*6^(k-1) */
        return (seq_t)5 * (seq_t)k * pow;
    } else {                       /* odd case: need 6^k */
        for (idx_t i = 0; i < k; ++i)
            pow *= (seq_t)6;
        /* result = (5*k+2)*6^k/2  */
        seq_t num = ((seq_t)5 * (seq_t)k + (seq_t)2) * pow;
        return num >> 1;           /* divide by 2 */
    }
}
