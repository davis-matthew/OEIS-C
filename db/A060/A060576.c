/*
 * A060576  a(n) = 1 except for a(1) = 0.
 * https://oeis.org/A060576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0; a(n)=1 for n > 1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is the only zero; all other indices produce 1 */
    if (n == (idx_t)1)
        return (seq_t)0;
    else
        return (seq_t)1;
}
