/*
 * A172531  Number of ways to place 4 nonattacking knights on an n X n
 *          toroidal board.
 * https://oeis.org/A172531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^6 - 54*n^4 + 1115*n^2 - 8934)/24 (n ≥ 9)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 24, u64 n <= 380, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         a(1) = 0, a(2) = 0, a(3) = 0, a(4) = 228,
 *         a(5) = 600, a(6) = 12357, a(7) = 68796, a(8) = 275888 */
seq_t a(idx_t n)
{
    /* Small pre‑computed values for n ≤ 8 */
    if (n <= 8)
    {
        static const seq_t table[8] = {0, 0, 0, 228, 600, 12357, 68796, 275888};
        return table[n - 1];
    }

    /* Use 128‑bit arithmetic for the general formula.
     * The intermediate values fit comfortably in 128 bits for all n ≤ 10 000. */
    unsigned __int128 nn   = (unsigned __int128)n;
    unsigned __int128 n2   = nn * nn;
    unsigned __int128 n4   = n2 * n2;
    unsigned __int128 n6   = n4 * n2;

    unsigned __int128 poly = n6
                            - (unsigned __int128)54 * n4
                            + (unsigned __int128)1115 * n2
                            - (unsigned __int128)8934;

    unsigned __int128 res128 = n2 * poly / 24;

    return (seq_t)res128;
}
