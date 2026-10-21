/*
 * A228996  Total sum of the 6th powers of lengths of ascending runs in all
 *          permutations of [n].
 * https://oeis.org/A228996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (derived from the EGF):
   a(0) = 0,  a(1) = 1
   For n ≥ 2,
   a(n) = 62 + 60·n·(n-2)
           + 60·n·(n-1)·(n-2)
           + 30·n·(n-1)·(n-2)·(n-3)
           + 2·n·a(n-1) - n·(n-1)·a(n-2)        */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t prev2 = 0;   /* a(n-2) */
    seq_t prev1 = 1;   /* a(n-1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t I      = (seq_t)i;
        seq_t term1  = 0;
        seq_t term2  = 0;
        seq_t term3  = 0;

        /* term1 = 60·n·(n-2) */
        if (i >= 2)
        {
            seq_t I2 = I - 2;               /* safe when i ≥ 2 */
            term1 = (seq_t)60 * I * I2;
        }

        /* term2 = 60·n·(n-1)·(n-2) */
        if (i >= 3)
        {
            seq_t I1 = I - 1;               /* safe when i ≥ 3 */
            seq_t I2 = I - 2;               /* safe when i ≥ 3 */
            term2 = (seq_t)60 * I * I1 * I2;
        }

        /* term3 = 30·n·(n-1)·(n-2)·(n-3) */
        if (i >= 4)
        {
            seq_t I1 = I - 1;               /* safe when i ≥ 4 */
            seq_t I2 = I - 2;               /* safe when i ≥ 4 */
            seq_t I3 = I - 3;               /* safe when i ≥ 4 */
            term3 = (seq_t)30 * I * I1 * I2 * I3;
        }

        seq_t S = (seq_t)62 + term1 + term2 + term3;

        seq_t two_i_prev1 = (seq_t)2 * I * prev1;
        seq_t n_nm1_prev2 = I * (I - 1) * prev2;

        seq_t curr = S + two_i_prev1 - n_nm1_prev2;

        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
