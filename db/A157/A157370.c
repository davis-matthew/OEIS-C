/*
 * A157370  a(n) = 2401*n^2 - 3822*n + 1520.
 * https://oeis.org/A157370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2401*n^2 - 3822*n + 1520
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 1338, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined for n < 1, return 0 */

    seq_t nn      = (seq_t)n;           /* cast index to seq_t */
    seq_t result  = (seq_t)2401 * nn;    /* 2401*n */
    result = result * nn;               /* 2401*n^2 */
    result = result - ((seq_t)3822 * nn); /* subtract 3822*n */
    result = result + (seq_t)1520;      /* add 1520 */
    return result;
}
