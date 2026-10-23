/*
 * A010716  Constant sequence: the all 5's sequence.
 * https://oeis.org/A010716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) { 
    (void)n;  /* silence unused parameter warning */
    return (seq_t)5; 
}
