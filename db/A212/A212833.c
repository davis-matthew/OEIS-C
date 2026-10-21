/*
 * A212833  Number of 0..n arrays of length n+1 with 0 never adjacent to n.
 * https://oeis.org/A212833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of arrays of length n+1 over the alphabet {0..n} with
 *            the restriction that 0 and n cannot be adjacent. Recurrence: Let
 *            Z_k, N_k, O_k be the counts of arrays of length k ending with 0,
 *            n, or any other symbol respectively. Initial values (length 1):
 *            Z_1 = 1, N_1 = 1, O_1 = n – 1 . For k =...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                        /* values below offset are irrelevant */

    /* The number of symbols other than 0 and n. */
    const seq_t T = (seq_t)(n - 1);

    /* DP state for length 1. */
    seq_t Z = 1;          /* endings with 0          */
    seq_t N = 1;          /* endings with n          */
    seq_t O = T;          /* endings with an “other” */

    /*
     * Perform n steps.  After the initial state (length 1) each step
     * increases the length by one.  After n steps the arrays have
     * length n+1, which is the required length.
     */
    for (idx_t i = 0; i < n; ++i)
    {
        const seq_t newZ = Z + O;
        const seq_t newN = N + O;
        const seq_t newO = (Z + N + O) * T;

        Z = newZ;
        N = newN;
        O = newO;
    }

    return Z + N + O;      /* total number of valid arrays */
}
