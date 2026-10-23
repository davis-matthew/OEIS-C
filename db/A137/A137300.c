/*
 * A137300  4 X 4 Latin square or "Vigenere" square read by rows.
 * https://oeis.org/A137300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, fini, full
 * Formula:   a(n) = ((k-2) mod 4)+1, where k is the diagonal sum (row+column)
 *            of the n‑th cell in the anti‑diagonal enumeration of the 4×4
 *            Vigenère (Latin) square
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety: offset is 1 */

    /* repeat the 4×4 block periodically */
    idx_t idx = (n - 1) % 16 + 1;

    /* determine the diagonal number k (sum of coordinates) */
    idx_t k;
    if (idx <= 1)          k = 2;
    else if (idx <= 3)     k = 3;
    else if (idx <= 6)     k = 4;
    else if (idx <= 10)    k = 5;
    else if (idx <= 13)    k = 6;
    else if (idx <= 15)    k = 7;
    else                   k = 8;

    /* the value on that diagonal is ((k-2) mod 4)+1 */
    seq_t val = (seq_t)((k - 2) % 4 + 1);
    return val;
}
