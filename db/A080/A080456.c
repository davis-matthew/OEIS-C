/*
 * A080456  a(1) = a(2) = 2; for n > 2, a(n) = a(n-1) if n is already in the
 *          sequence, a(n) = a(n-1) + 4 otherwise.
 * https://oeis.org/A080456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=a(2)=2; for 3≤n≤6: a(n)=2+4·(n−2); for n>6: a(n)=a(n−1)+4
 *            unless n≡3 (mod 4), in which case a(n)=a(n−1). Closed form for
 *            n>6: let z=⌊(n−7)/4⌋+1 (number of indices i∈[7,n] with i≡3 mod
 *            4); let nz=(n−6)−z; then a(n)=18+4·nz. This formula matches the
 *            OEIS definition and is O(1)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)2;
    if (n <= 6)
        return (seq_t)2 + (seq_t)4 * (n - 2);
    /* n > 6 */
    idx_t z = ((n - 7) / 4) + 1;            /* count of zeros (indices ≡3 mod 4) */
    idx_t nz = (n - 6) - z;                 /* count of non‑zero steps */
    return (seq_t)18 + (seq_t)4 * nz;
}
