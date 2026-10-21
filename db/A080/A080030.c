/*
 * A080030  a(n) is taken to be the smallest positive integer not already
 *          present which is consistent with the condition "n is a member of
 *          the sequence if and only if a(n) is congruent to 1 mod 3".
 * https://oeis.org/A080030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n+2 if n mod 3 == 0, 2*n-1 if n mod 3 == 1, 2*n if n mod
 *            3 == 2
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;   /* promote index to seq_t for arithmetic */
    switch ((uint64_t)(n % 3))   /* n is uint64_t; modulo yields uint64_t */
    {
        case 0:
            return nn + (seq_t)2;
        case 1:
            return nn * (seq_t)2 - (seq_t)1;
        default:    /* case 2 */
            return nn * (seq_t)2;
    }
}
