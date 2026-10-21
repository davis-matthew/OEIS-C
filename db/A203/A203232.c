/*
 * A203232  (n-1)-st elementary symmetric function of the first n terms of the
 *          periodic sequence (2,3,2,3,2,3,...).
 * https://oeis.org/A203232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-2) - 36*a(n-4) for n >= 5 with initial values
 *            a(1)=1, a(2)=5, a(3)=16, a(4)=60. This implements the elementary
 *            symmetric function definition for the periodic sequence
 *            (2,3,2,3, …) without using a lookup table
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* value below the offset is irrelevant */

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)16;
    if (n == 4) return (seq_t)60;

    /* Hold a(i-4), a(i-3), a(i-2), a(i-1) while iterating */
    seq_t a1 = (seq_t)1;  /* a(1)   */
    seq_t a2 = (seq_t)5;  /* a(2)   */
    seq_t a3 = (seq_t)16; /* a(3)   */
    seq_t a4 = (seq_t)60; /* a(4)   */
    seq_t curr;

    for (idx_t i = 5; i <= n; ++i)
    {
        /* a(i) = 12*a(i-2) - 36*a(i-4) */
        curr = (seq_t)12 * a3 - (seq_t)36 * a1;

        /* shift the window for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = curr;
    }

    return a4;  /* a(n) */
}
