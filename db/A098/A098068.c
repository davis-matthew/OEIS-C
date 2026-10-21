/*
 * A098068  If C is a doubly-even binary code of length N=24m+n, 1 <= n <= 24,
 *          then the minimum distance of C^perp is bounded above by 4m+a(n).
 * https://oeis.org/A098068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 4*⌊(n‑1)/24⌋ + b[(n‑1) mod 24] where b =
 *            {1,1,1,2,1,2,3,4,1,2,2,2,2,3,3,4,4,4,3,4,5,6,7,8}
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Indexing constant table of a(n) for n = 1…24. */
    static const seq_t b[24] = {
        1, 1, 1, 2, 1, 2, 3, 4,
        1, 2, 2, 2, 2, 3, 3, 4,
        4, 4, 3, 4, 5, 6, 7, 8
    };
    /* Compute the quotient and remainder of (n-1) divided by 24. */
    idx_t period = 24u;
    idx_t q = (n - 1u) / period;      /* floor((n-1)/24)  */
    idx_t r = (n - 1u) % period;      /* (n-1) mod 24     */
    /* Add the periodic offset 4*q to the base value. */
    return b[r] + (seq_t)(4 * q);
}
