/*
 * A105676  Highest minimal Hamming distance of any Type 3 ternary self-dual
 *          code of length 4n.
 * https://oeis.org/A105676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 3 * ceil((n+1)/3)
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Using integer arithmetic: ceil((n+1)/3) = (n+3)/3 */
    seq_t temp = (seq_t)n + 3;     /* promote n to seq_t, add 3 */
    return 3 * (temp / 3);
}
