/*
 * A025811  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^11)).
 * https://oeis.org/A025811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A025811 – Partitions of n into 2, 5, 11.
   Counts solutions to 2*i + 5*j + 11*k = n with i,j,k ≥ 0. */
seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n / 11; ++k) {
        idx_t r   = n - 11 * k;          /* remaining amount after 11*k */
        idx_t jmax = r / 5;              /* maximum j such that 5*j ≤ r */
        idx_t parity = r & 1;            /* r mod 2 */

        if (jmax < parity) continue;    /* no j with correct parity */

        /* number of j values: first j = parity, then step 2 */
        idx_t count_j = 1 + (jmax - parity) / 2;
        total += (seq_t)count_j;
    }
    return total;
}
