/*
 * A151899  Period 6: repeat [0, 0, 1, 1, 1, 2].
 * https://oeis.org/A151899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6) for n>5; a(0)=0, a(1)=0, a(2)=1, a(3)=1, a(4)=1,
 *            a(5)=2
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It repeats [0, 0, 1, 1, 1, 2] every 6 terms. */
seq_t a(idx_t n)
{
    switch (n % 6) {
        case 0: case 1: return (seq_t)0;
        case 2: case 3: case 4: return (seq_t)1;
        case 5: return (seq_t)2;
        default: return (seq_t)0; /* unreachable */
    }
}
