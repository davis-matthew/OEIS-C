/*
 * A195047  Concentric 17-gonal numbers.
 * https://oeis.org/A195047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (34*n^2 + 13*(-1)^n - 13) / 8
 * Range:     u8 n <= 2, u16 n <= 43, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t base = (((seq_t)34) * nn * nn);   /* 34*n^2 */
    seq_t num;
    if ((n & 1) == 0)           /* n even: (-1)^n = 1 -> add 0 */
        num = base;
    else                        /* n odd:  (-1)^n = -1 -> subtract 26 */
        num = base - 26;
    return num / 8;
}
