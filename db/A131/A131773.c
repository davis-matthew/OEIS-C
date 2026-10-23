/*
 * A131773  Epact in Julian calendar for a year n with Golden Number (n mod
 *          19) + 1 = A074805(n).
 * https://oeis.org/A131773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11 * (n mod 19)) mod 30, with 0 replaced by 30
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    uint64_t r = n % 19;          /* n mod 19 */
    seq_t m = ((seq_t)11 * (seq_t)r) % (seq_t)30;
    return m ? m : (seq_t)30;    /* replace 0 with 30 */
}
