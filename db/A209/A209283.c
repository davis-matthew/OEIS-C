/*
 * A209283  a(n) is difference of indices of terms >= 6 in A187824.
 * https://oeis.org/A209283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence: a(n) = c[(n-1) mod 15] where
   c = {6, 8, 5, 1, 4, 2, 4, 1, 5, 8, 6, 4, 1, 1, 4}. */
seq_t a(idx_t n)
{
    static const seq_t cycle[15] = {6, 8, 5, 1, 4, 2,
                                    4, 1, 5, 8, 6, 4,
                                    1, 1, 4};
    /* n is a 64‑bit unsigned index starting at 1 */
    idx_t idx = (n - 1) % 15;
    return cycle[idx];
}
