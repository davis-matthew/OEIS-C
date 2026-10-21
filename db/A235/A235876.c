/*
 * A235876  Number of (n+1)X(n+1) 0..2 arrays with the minimum plus the upper
 *          median equal to the lower median plus the maximum in every 2X2
 *          subblock.
 * https://oeis.org/A235876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 35*a(n-2) + 50*a(n-3) - 24*a(n-4) with a(1) =
 *            21, a(2) = 105, a(3) = 453, a(4) = 1833
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage function that returns the nth term of OEIS sequence A235876. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* undefined for n < 1, return 0 */

    /* Initial terms for n = 1 .. 4 */
    switch (n) {
        case 1: return (seq_t)21;
        case 2: return (seq_t)105;
        case 3: return (seq_t)453;
        case 4: return (seq_t)1833;
    }

    /* Compute successive terms up to n using the recurrence. */
    seq_t a1 = (seq_t)21;     /* a(n-4) */
    seq_t a2 = (seq_t)105;    /* a(n-3) */
    seq_t a3 = (seq_t)453;    /* a(n-2) */
    seq_t a4 = (seq_t)1833;   /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next =
            (seq_t)10 * a4     /* 10 * a(n-1) */
            - (seq_t)35 * a3   /* 35 * a(n-2) */
            + (seq_t)50 * a2   /* 50 * a(n-3) */
            - (seq_t)24 * a1;  /* 24 * a(n-4) */

        /* Shift window: a(n-3) becomes a(n-4), etc. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}
