/*
 * A080151  Let m = Wonderful Demlo number A002477(n); a(n) = sum of digits of
 *          m.
 * https://oeis.org/A080151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 81 * ((n-1)/9) + ( ((n-1)%9) + 1 )^2
 * Range:     u8 n <= 30, u16 n <= 7283, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute block number and position inside block */
    idx_t block = (n - 1) / 9;          /* q = ⌊(n-1)/9⌋   */
    idx_t pos   = (n - 1) % 9;          /* r = (n-1) mod 9 */

    /* Using seq_t for the final result, cast intermediate values if needed */
    seq_t result = (seq_t)81 * block;
    result += (seq_t)(pos + 1) * (seq_t)(pos + 1);
    return result;
}
