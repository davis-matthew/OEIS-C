/*
 * A041110  Numerators of continued fraction convergents to sqrt(63).
 * https://oeis.org/A041110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 16*a(n-2) - a(n-4).
 * Base terms: a(0)=7, a(1)=8, a(2)=119, a(3)=127.
 */
seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)7;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)119;
    if (n == 3) return (seq_t)127;

    /* Last four values of the sequence.
       They are kept in a circular buffer of size 4. */
    seq_t buf[4] = {(seq_t)7, (seq_t)8, (seq_t)119, (seq_t)127};

    /* Compute terms up to n */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t prev2 = buf[(i-2) & 3];
        seq_t prev4 = buf[(i-4) & 3];
        buf[i & 3] = (prev2 * (seq_t)16) - prev4;
    }

    /* Return the requested term */
    return buf[n & 3];
}
