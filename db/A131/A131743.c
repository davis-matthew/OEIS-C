/*
 * A131743  Period 4: repeat [0, 1, 0, 2].
 * https://oeis.org/A131743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n) mod 3 (equivalently a(n)=a(n-4) with
 *            a(0)=0,a(1)=1,a(2)=0,a(3)=2)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pattern repeats every 4: 0,1,0,2 */
    unsigned int r = (unsigned int)(n & 3ULL);  /* n % 4 */
    switch (r) {
        case 1:  return (seq_t)1;
        case 3:  return (seq_t)2;
        default: return (seq_t)0;
    }
}
