/*
 * A169713  The function W_n(10) (see Borwein et al. reference for
 *          definition).
 * https://oeis.org/A169713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 120*n^5 - 600*n^4 + 1250*n^3 - 1225*n^2 + 456*n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 33, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1. For n == 0 return 0. */
    if (n == 0)
        return 0;

    /* Use 128‑bit signed arithmetic to avoid overflow of intermediate
       values while keeping the final result exact when it fits in seq_t. */
    __int128 N = (__int128)n;

    __int128 res =
        120 * N * N * N * N * N
        - 600 * N * N * N * N
        + 1250 * N * N * N
        - 1225 * N * N
        + 456  * N;

    return (seq_t)res;
}
