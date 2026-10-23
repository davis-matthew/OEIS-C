/*
 * A141449  A005439 mod 9.
 * https://oeis.org/A141449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A141449: a(0)=1, a(1)=1, a(2)=2, and for n>=3 values repeat 8,2,5 with period 3. */
seq_t a(idx_t n)
{
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)1;
        case 2: return (seq_t)2;
        default:
            /* n >= 3 */
            idx_t r = (n - 3) % 3;
            return (seq_t)(r == 0 ? 8 : (r == 1 ? 2 : 5));
    }
}
