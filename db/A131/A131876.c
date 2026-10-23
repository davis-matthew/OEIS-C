/*
 * A131876  2*A131873 - 1.
 * https://oeis.org/A131876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * Let r = ⌊(√(8n+1) – 1)/2⌋ (the row such that T_r ≤ n <
 *            T_{r+1}, T_r = r(r+1)/2), * let k = n – T_r (column index, 0 ≤ k
 *            ≤ r). * Then * a(n) = 14·r + 1 if k = 0 * a(n) = 8·r – 2·k + 1
 *            otherwise. * (This is 2·A131873(n) – 1.)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r with T_r ≤ n < T_{r+1}.  For n up to 10 000
       a linear scan is ample; r is at most about √(2n) < 150. */
    idx_t r = 0;
    /* Compute T_r = r*(r+1)/2.  All intermediate quantities fit in
       64 bit for the required range. */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within the row. */
    idx_t k = n - r * (r + 1) / 2;

    /* Compute the value using only integer arithmetic. */
    if (k == 0)
        return (seq_t)14 * (seq_t)r + 1;
    else
        return (seq_t)8 * (seq_t)r - (seq_t)2 * (seq_t)k + 1;
}
