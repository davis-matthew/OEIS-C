/*
 * A128990  a(n) = (n^3 - n^2)*7^n.
 * https://oeis.org/A128990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n^2)*7^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A128990 – offset 1 */

seq_t a(idx_t n)
{
    /* Compute 7^n using exponentiation by squaring */
    seq_t pow7 = 1;    /* result */
    seq_t base = 7;    /* base 7 */
    idx_t e = n;       /* exponent, in idx_t to avoid overflow on shifts */

    while (e) {
        if (e & 1) {
            pow7 *= base;
        }
        base *= base;
        e >>= 1;
    }

    /* Compute n^2 * (n - 1) = n^3 - n^2 */
    seq_t n_seq = (seq_t)n;          /* convert index to seq_t once */
    seq_t n_sq  = n_seq * n_seq;     /* n^2  */
    seq_t poly  = n_sq * (seq_t)(n - 1); /* n^2 * (n-1) */

    /* Final result */
    return poly * pow7;
}
