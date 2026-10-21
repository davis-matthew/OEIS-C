/*
 * A236398  Period 4: repeat 1,1,2,1.
 * https://oeis.org/A236398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) for n>4; initial values a(1)=1, a(2)=1, a(3)=2,
 *            a(4)=1
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = (n - 1) & 3;  /* (n-1) mod 4, as 4 is a power of two */
    switch (m) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)1;
        case 2: return (seq_t)2;
        default: return (seq_t)1;  /* case 3 */
    }
}
