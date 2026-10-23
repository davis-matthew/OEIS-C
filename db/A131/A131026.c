/*
 * A131026  Periodic sequence (2, 2, 1, 0, 0, 1).
 * https://oeis.org/A131026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6) with initial terms a(1)=2, a(2)=2, a(3)=1, a(4)=0,
 *            a(5)=0, a(6)=1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 6;
    switch (r) {
    case 1: case 2: return (seq_t)2;
    case 3:            return (seq_t)1;
    case 4: case 5: return (seq_t)0;
    case 0:            return (seq_t)1;
    }
    return (seq_t)0; /* unreachable */
}
