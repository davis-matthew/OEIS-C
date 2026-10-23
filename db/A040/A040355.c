/*
 * A040355  Continued fraction for sqrt(375).
 * https://oeis.org/A040355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic continued fraction terms for sqrt(375).  */
seq_t a(idx_t n)
{
    static const unsigned short period[10] = {2,1,2,1,5,1,2,1,2,38};
    if (n == 0u)
        return (seq_t)19;
    /* Compute offset within period (period length = 10) */
    idx_t offset = (n - 1u) % 10u;
    return (seq_t)period[offset];
}
