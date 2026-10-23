/*
 * A014738  Squares of even triangular numbers.
 * https://oeis.org/A014738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( T_k )^2 where k is the (n+1)-th index of an even
 *            triangular number (k = 2*n + 3 - (n % 2)) and T_k = k*(k+1)/2
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 179, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid unintended truncation. */
    seq_t ns = (seq_t)n;

    /* k = 2*n + 3 - (n & 1) produces the sequence 3,4,7,8,11,12,... */
    seq_t k = (ns << 1) + (seq_t)3 - (ns & (seq_t)1);

    /* Triangle number t = k*(k+1)/2. */
    seq_t t = k * (k + (seq_t)1) / (seq_t)2;

    /* Return its square. */
    return t * t;
}
