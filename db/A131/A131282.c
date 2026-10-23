/*
 * A131282  Period 6: repeat [1, 2, 3, 3, 4, 5].
 * https://oeis.org/A131282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6) for n>5; a(0)=1, a(1)=2, a(2)=3, a(3)=3, a(4)=4,
 *            a(5)=5
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % (idx_t)6;
    switch (r)
    {
        case 0: return (seq_t)1;
        case 1: return (seq_t)2;
        case 2: return (seq_t)3;
        case 3: return (seq_t)3;
        case 4: return (seq_t)4;
        case 5: return (seq_t)5;
        default: return (seq_t)0;    /* unreachable */
    }
}
