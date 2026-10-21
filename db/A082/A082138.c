/*
 * A082138  A transform of C(n,3).
 * https://oeis.org/A082138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n+3,3) * 2^(n-1) , a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 50, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 1 by definition */
        return (seq_t)1;

    /* Compute C(n+3,3) = (n+3)(n+2)(n+1)/6 */
    seq_t comb = (seq_t)(n + 3);
    comb *= (seq_t)(n + 2);
    comb *= (seq_t)(n + 1);
    comb /= (seq_t)6;          /* exact division */

    /* 2^(n-1): shift by n-1 bits */
    seq_t pow2 = (seq_t)1 << (n - 1);

    return comb * pow2;
}
