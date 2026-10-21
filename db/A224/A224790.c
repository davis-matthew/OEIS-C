/*
 * A224790  a(n) = 3*9^n + 8.
 * https://oeis.org/A224790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*9^n + 8
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Exponentiation by squaring: compute 9^n in seq_t. */
    seq_t result;
    seq_t pow9 = 1;   /* current product 9^k */
    seq_t base = 9;   /* current base 9^(2^i) */
    idx_t e = n;

    while (e)
    {
        if (e & 1)     /* if the current least‑significant bit is set */
            pow9 *= base;
        base *= base;  /* square the base for the next bit */
        e >>= 1;       /* shift to process next bit */
    }

    /* a(n) = 3*9^n + 8 */
    result = 3 * pow9 + 8;
    return result;
}
