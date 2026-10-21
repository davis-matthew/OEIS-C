/*
 * A215960  First differences of A016759.
 * https://oeis.org/A215960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(7*t*(t*(t+17)+91)+1093), where t = 4*n*(n+2)
 * Range:     u8 none, u16 n <= 0, u32 n <= 11, u64 n <= 522, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;                  /* promote index to seq_t */
    seq_t t   = (seq_t)4 * ns * (ns + (seq_t)2);  /* t = 4*n*(n+2) */
    seq_t inner = t * (t + (seq_t)17) + (seq_t)91;/* inner = t*(t+17)+91 */
    seq_t temp   = (seq_t)7 * t * inner + (seq_t)1093; /* 7*t*inner + 1093 */
    return (seq_t)2 * temp;               /* final result */
}
