/*
 * A070574  a(n) = n^4 mod 31.
 * https://oeis.org/A070574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 31
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n modulo 31 using idx_t arithmetic to avoid truncation */
    idx_t r0 = n % 31u;          /* r0 is 0 <= r0 < 31 */
    seq_t r = (seq_t)r0;         /* cast to seq_t for further operations */

    /* Compute r^4 modulo 31 */
    seq_t r2 = (r * r) % 31;      /* r^2 mod 31 */
    seq_t r4 = (r2 * r2) % 31;    /* (r^2)^2 mod 31 */

    return r4;
}
