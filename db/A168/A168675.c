/*
 * A168675  a(n) = n^8*(n + 1)/2.
 * https://oeis.org/A168675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8*(n+1)/2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 149, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to seq_t before any arithmetic. */
    seq_t nn = (seq_t)n;

    /* Compute n^8 as (((n^2)^2)^2). */
    seq_t n2 = nn * nn;
    seq_t n4 = n2 * n2;
    seq_t n8 = n4 * n4;

    if ((n & 1) == 0) {
        /* n is even: n^8 is divisible by 2^8, safely divide first. */
        seq_t half = n8 / 2;
        return half * (nn + 1);
    } else {
        /* n is odd: divide (n+1) by 2 first. */
        seq_t half = (nn + 1) / 2;
        return n8 * half;
    }
}
