/*
 * A087945  Expansion of (1-2x-x^2)/((1-2x)(1-4x+x^2)).
 * https://oeis.org/A087945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) + 2*a(n-3), with a(0) = 1, a(1) = 4,
 *            a(2) = 14
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)14;

    /* Use a small running window to avoid storing the whole sequence. */
    seq_t a0 = (seq_t)1;   /* a(n-3) for the first loop iteration */
    seq_t a1 = (seq_t)4;   /* a(n-2) */
    seq_t a2 = (seq_t)14;  /* a(n-1) */
    seq_t a3;              /* a(n) */

    for (idx_t i = 3; i <= n; ++i)
    {
        /* Perform the recurrence using an intermediate type large
           enough to hold the product without overflow. */
        a3 = (seq_t)(((__uint128_t)6 * a2 -
                      (__uint128_t)9 * a1 +
                      (__uint128_t)2 * a0));

        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a2; /* a(n) is stored in a2 after the loop */
}
