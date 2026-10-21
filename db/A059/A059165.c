/*
 * A059165  a(n) = (n+1)*2^(n+4).
 * https://oeis.org/A059165
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)*2^(n+4) (offset -1)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 23, u64 n <= 54, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^(n+4) by repeated doubling (addition). */
    seq_t pow2 = (seq_t)1;
    idx_t exp = n + 4;
    for (idx_t i = 0; i < exp; ++i)
        pow2 += pow2;            /* safe for unsigned types (wraps modulo 2^width) */

    /* Multiply by (n+1).  Cast to seq_t to avoid signed/unsigned mix. */
    seq_t coeff = (seq_t)(n + 1);
    return coeff * pow2;
}
