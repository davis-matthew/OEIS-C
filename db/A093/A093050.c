/*
 * A093050  Exponent of 2 in (3^n-3)*2^(n-1).
 * https://oeis.org/A093050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = { 0, if n = 0 or 1; n-1, if n is odd; a(n/2) + n/2 + 1 +
 *            ( (n/2) mod 2 ), if n is even >= 2. }
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    if ((n & 1))
        return (seq_t)(n - 1);

    idx_t k = n >> 1; /* k = n/2 */
    seq_t ak = a(k);
    /* add k (as seq_t), constant 1, and 1 more if k is odd */
    return ak + (seq_t)k + 1 + (seq_t)(k & 1);
}
