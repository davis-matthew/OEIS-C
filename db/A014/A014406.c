/*
 * A014406  Number of strictly increasing arithmetic progressions of positive
 *          integers with at least 3 terms and sum <= n.
 * https://oeis.org/A014406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 799, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of strictly increasing arithmetic progressions of positive integers
   with length at least 3 whose sum does not exceed n.  The algorithm
   iterates over all admissible lengths L, then counts all pairs of starting
   term a ≥ 1 and difference d ≥ 1 such that L·a + d·L·(L−1)/2 ≤ n.  The
   result is computed directly in O(n log n) time and uses only integer
   arithmetic. */

seq_t a(idx_t n)
{
    if (n < 6)                     /* smallest sum for a 3‑term AP is 1+2+3=6 */
        return 0;

    seq_t count = 0;
    for (idx_t L = 3;; ++L)
    {
        /* Check if the minimal sum for this length exceeds n */
        seq_t T_L = (seq_t)L * ((seq_t)L + 1) / 2;   /* triangular number */
        if (T_L > n)
            break;

        /* B = L*(L-1)/2 is the coefficient of d in the sum expression */
        seq_t B = (seq_t)L * ((seq_t)L - 1) / 2;

        /* Maximum starting term a such that some d≥1 is possible */
        seq_t a_max = (n - B) / L;
        if (a_max == 0)
            continue;

        for (idx_t a = 1; a <= (idx_t)a_max; ++a)
        {
            /* Maximal difference d that keeps the sum <= n */
            seq_t d_max = (n - (seq_t)L * a) / B;
            if (d_max > 0)
                count += d_max;
        }
    }

    return count;
}
