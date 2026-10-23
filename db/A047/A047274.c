/*
 * A047274  Numbers that are congruent to {0, 1} mod 7.
 * https://oeis.org/A047274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=1, a(n)=7*((n-1)/2)+(n%2==0?1:0) for n>=1
 * Range:     u8 n <= 74, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Assume n>=1; if n==0 this would wrap but the sequence is
       defined only for n>=1. */
    seq_t k = (seq_t)(n-1) / 2;                 /* floor((n-1)/2)   */
    seq_t result = (seq_t)7 * k;                /* 7*k              */
    if ((n & 1) == 0) {                       /* n even → add 1   */
        result += 1;
    }
    return result;
}
