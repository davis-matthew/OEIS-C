/*
 * A014391  Final digit of 8^n.
 * https://oeis.org/A014391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n mod 10
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Cycle length 4: 8,4,2,6 for n mod 4 = 1,2,3,0 (n>0) */
    idx_t r = n % (idx_t)4;
    switch (r) {
        case (idx_t)0: return (seq_t)6; /* n > 0 and n % 4 == 0 */
        case (idx_t)1: return (seq_t)8;
        case (idx_t)2: return (seq_t)4;
        case (idx_t)3: return (seq_t)2;
        default:       return (seq_t)0; /* unreachable */
    }
}
