/*
 * A220509  a(n) = n^3 + 3*n + 3^n.
 * https://oeis.org/A220509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + 3*n + 3^n
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;      /* promote integer index to seq_t */
    seq_t n3 = nn * nn * nn;  /* n^3      */
    seq_t t1 = 3 * nn;        /* 3*n      */

    /* Compute 3^n by exponentiation by squaring, avoiding a helper. */
    seq_t pow3 = 1;           /* result */
    seq_t base = 3;           /* current power of 3 */
    idx_t   exp  = n;         /* exponent (must stay as idx_t for shifting) */
    while (exp > 0) {
        if (exp & 1)
            pow3 *= base;
        base *= base;
        exp >>= 1;
    }

    return n3 + t1 + pow3;    /* n^3 + 3*n + 3^n */
}
