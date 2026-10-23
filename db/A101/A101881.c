/*
 * A101881  Write two numbers, skip one, write two, skip two, write two, skip
 *          three ... and so on.
 * https://oeis.org/A101881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/16)*(2*n^2 + 18*n + 15 + (2*n+1)*(-1)^n)
 * Range:     u8 n <= 41, u16 n <= 719, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent simplified form: for n even   a = ((k*k + 5*k + 2) >> 1) */
/*                            for n odd    a = ((k*k + 5*k + 4) >> 1) */
/* where k = floor(n/2). */

seq_t a(idx_t n)
{
    /* k = floor(n/2) */
    seq_t k = (seq_t)(n >> 1);

    if ((n & 1) == 0)      /* n is even */
    {
        return (((k * k) + (5 * k) + 2) >> 1);
    }
    else                   /* n is odd  */
    {
        return (((k * k) + (5 * k) + 4) >> 1);
    }
}
