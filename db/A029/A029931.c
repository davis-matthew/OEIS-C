/*
 * A029931  If 2n = Sum 2^e_i, a(n) = Sum e_i.
 * https://oeis.org/A029931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, tabf, look
 * Formula:   a(n) = sum_{i>=0} (i+1) * ((n>>i) & 1)
 * Range:     u8 n <= 1023, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the sum of the exponents of the powers of two in the binary
   representation of 2n; equivalently, the sum of (position+1) for
   each set bit in n. */

seq_t a(idx_t n)
{
    seq_t sum = 0;      /* result */
    idx_t  pos = 1;     /* position index (1-based) */

    /* scan bits of n */
    while (n) {
        if (n & 1)
            sum += (seq_t)pos;   /* add position when bit is set */
        n >>= 1;
        pos++;
    }

    return sum;
}
