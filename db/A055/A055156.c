/*
 * A055156  Powers of 3 which are not powers of 3^3.
 * https://oeis.org/A055156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^( 3*floor(n/2) + (n%2?2:1) )
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of A055156: powers of 3 whose exponents are not multiples of 3.
   Uses exponentiation by squaring on the type seq_t. */

seq_t a(idx_t n)
{
    /* Compute the exponent of 3 for the n-th term */
    idx_t e = 3 * (n >> 1) + (n & 1 ? 2 : 1);

    /* Fast exponentiation: result = 3^e */
    seq_t result = 1;
    seq_t base   = 3;

    while (e) {
        if (e & 1)
            result = result * base;
        base = base * base;
        e >>= 1;
    }
    return result;
}
