/*
 * A061101  Squares with digital root 7.
 * https://oeis.org/A061101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (9*n-8)^2/4 for n even, (9*n-1)^2/4 for n odd
 * Range:     u8 n <= 4, u16 n <= 56, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1; 0 is unused */
    seq_t x = (seq_t)n;
    seq_t t;
    if ((n & 1) == 0) {
        t = (seq_t)9 * x - (seq_t)8;
    } else {
        t = (seq_t)9 * x - (seq_t)1;
    }
    seq_t half = t / (seq_t)2;
    return half * half;
}
