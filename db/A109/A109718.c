/*
 * A109718  Periodic sequence with period {0,1,0,3}, or n^3 mod 4.
 * https://oeis.org/A109718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^3 mod 4
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 4 – the pattern of n^3 mod 4 repeats every 4 numbers. */
    idx_t r = n & 3;          /* equivalent to n % 4 for unsigned integers */

    switch (r) {
        case 1: return 1;     /* 1^3 mod 4 = 1 */
        case 3: return 3;     /* 3^3 mod 4 = 27 mod 4 = 3 */
        default: return 0;    /* for r = 0 or 2, n^3 mod 4 = 0 */
    }
}
