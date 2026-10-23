/*
 * A130766  3n+2 sandwiched by tripled 3n+1 .
 * https://oeis.org/A130766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(n/4) + floor((n+1)/4) + 1
 * Range:     u8 n <= 339, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A130766, Offset 0 */
seq_t a(idx_t n)
{
    /* All operations are performed in idx_t to avoid overflow,
       then converted to seq_t, safe for values that fit in seq_t. */
    seq_t q = n / 4;          /* floor(n/4) */
    seq_t r = (n + 1) / 4;    /* floor((n+1)/4) */
    return q * 2 + r + 1;     /* a(n) */
}
