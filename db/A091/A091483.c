/*
 * A091483  a(n) = (4*n)^n.
 * https://oeis.org/A091483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base = 4*n as seq_t */
    seq_t base = (seq_t)(4 * (seq_t)n);
    /* exponentiation by squaring (n may be 0) */
    seq_t result = (seq_t)1;
    seq_t b      = base;
    idx_t e      = n;

    while (e > 0)
    {
        if (e & 1)
            result = result * b;
        e >>= 1;
        if (e)
            b = b * b;
    }
    return result;
}
