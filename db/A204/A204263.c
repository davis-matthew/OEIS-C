/*
 * A204263  Symmetric matrix: T(i,j)=(i+j mod 3), by antidiagonals.
 * https://oeis.org/A204263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   a(n) = ((ceil((sqrt(8*n+1)-1)/2)+1) mod 3)
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal d such that d*(d+1)/2 ≥ n */
    idx_t d   = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++d;
        sum += d;
    }
    /* Each entry on anti‑diagonal d has i+j = d+1, hence value = (d+1) mod 3 */
    return (seq_t)((d + 1) % 3ULL);
}
