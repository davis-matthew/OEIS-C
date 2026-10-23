/*
 * A130823  Each odd number appears thrice.
 * https://oeis.org/A130823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor((n-1)/3)+1
 * Range:     u8 n <= 383, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 in all valid calls; cast to seq_t for the arithmetic */
    seq_t m = (seq_t)(n - 1);   /* (n-1) */
    m = m / 3;                  /* floor((n-1)/3) */
    m = m * 2;                  /* 2*floor((n-1)/3) */
    return 1 + m;               /* +1 */
}
