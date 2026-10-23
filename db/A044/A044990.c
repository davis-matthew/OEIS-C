/*
 * A044990  Numbers whose base-3 representation contains no 0's and exactly
 *          one 2.
 * https://oeis.org/A044990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (3^L – 1)/2 + 3^k, where * L is the smallest integer with
 *            L(L+1)/2 ≥ n, * and k = n – L(L–1)/2 – 1 (0‑based position in
 *            length L)
 * Range:     u8 n <= 15, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n = 0 */

    /* Find minimal L with triangular number ≥ n */
    idx_t L = 1;
    while ((L * (L + 1)) / 2 < n)
        ++L;

    /* Count of numbers with fewer digits */
    idx_t T_prev = (L - 1) * L / 2;

    /* Position within current block (0‑based) */
    idx_t pos = n - T_prev - 1;

    /* Compute 3^L */
    seq_t pow3L = 1;
    for (idx_t i = 0; i < L; ++i)
        pow3L *= 3;

    /* Base value with all digits 1 */
    seq_t base = (pow3L - 1) / 2;

    /* Compute 3^pos */
    seq_t pow3Pos = 1;
    for (idx_t i = 0; i < pos; ++i)
        pow3Pos *= 3;

    return base + pow3Pos;
}
