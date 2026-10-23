/*
 * A028772  Nonsquares mod 59.
 * https://oeis.org/A028772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 59*floor((n-1)/29) + r[(n-1)%29] where r =
 *            {2,6,8,10,11,13,14,18,23,24,30,31,32,33,34,37,38,39,40,42,43,44,47,50,52,54,55,56,58}
 * Range:     u8 n <= 29, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                   /* sequence starts at n = 1 */
        return (seq_t)0;

    /* Sorted list of quadratic nonresidues modulo 59. */
    static const unsigned char non_residues[29] = {
        2, 6, 8, 10, 11, 13, 14, 18, 23, 24,
        30, 31, 32, 33, 34, 37, 38, 39, 40, 42,
        43, 44, 47, 50, 52, 54, 55, 56, 58
    };

    /* Compute the number of complete 59‑periods before the desired term,
       and the position within the current period. */
    idx_t full_cycles = (n - 1) / 29;
    idx_t pos          = (n - 1) % 29;

    /* Assemble the nth term. */
    seq_t result = (seq_t)full_cycles * (seq_t)59 + (seq_t)non_residues[pos];
    return result;
}
