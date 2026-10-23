/*
 * A027617  Number of permutations of n elements containing a 3-cycle.
 * https://oeis.org/A027617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = n*a(n-1) - (n-2)*(n-1)*a(n-3) + (n-3)*(n-2)*(n-1)*a(n-4),
   a(0)=0, a(1)=0, a(2)=0, a(3)=2 */
seq_t a(idx_t n)
{
    /* Predefined initial terms */
    seq_t a0 = 0; /* a(0) */
    seq_t a1 = 0; /* a(1) */
    seq_t a2 = 0; /* a(2) */
    seq_t a3 = 2; /* a(3) */

    /* Handle the first four indices directly */
    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    /* Iterate using the recurrence, keeping only the last four values */
    for (idx_t i = 4; i <= n; ++i)
    {
        /* term1 = i * a(n-1) */
        seq_t term1 = (seq_t)i * a3;

        /* term2 = (i-2)*(i-1) * a(n-3) */
        seq_t coeff1 = (seq_t)(i - 2) * (seq_t)(i - 1);
        seq_t term2 = coeff1 * a1;

        /* term3 = (i-3)*(i-2)*(i-1) * a(n-4) */
        seq_t coeff2 = (seq_t)(i - 3) * (seq_t)(i - 2) * (seq_t)(i - 1);
        seq_t term3 = coeff2 * a0;

        /* Next value */
        seq_t next = term1 - term2 + term3;

        /* Shift values for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
