/*
 * A204421  Symmetric matrix: f(i,j)=(i+j+2 mod 3), by antidiagonals.
 * https://oeis.org/A204421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k + 3) mod 3, where k is the smallest integer with k*(k
 *            + 1)/2 >= n. This corresponds to the value on the n‑th
 *            anti‑diagonal of the infinite matrix f(i,j) = (i + j + 2) mod 3
 *            (with offset 1)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the minimal k such that k*(k+1)/2 >= n */
    idx_t k = 1;
    while (1) {
        unsigned __int128 cum = (unsigned __int128)k * (k + 1) / 2;
        if (cum >= n)
            break;
        ++k;
    }
    /* Value on that anti-diagonal: (k + 3) mod 3 */
    return (seq_t)((k + 3) % 3);
}
