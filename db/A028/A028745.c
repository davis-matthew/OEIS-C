/*
 * A028745  Nonsquares mod 32.
 * https://oeis.org/A028745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 32 * floor((n-1)/25) + r[(n-1) mod 25]
 * Range:     u8 n <= 25, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where r = {2,3,5,6,7,8,10,11,12,13,14,15,18,19,20,21,22,23,24,26,27,28,29,30,31} */

seq_t a(idx_t n)
{
    /* Non‑square residues modulo 32 (25 of them) */
    static const idx_t nonres[25] = {
        2, 3, 5, 6, 7, 8, 10, 11, 12, 13,
        14, 15, 18, 19, 20, 21, 22, 23, 24, 26,
        27, 28, 29, 30, 31
    };

    const idx_t per_cycle = 25;      /* 25 non‑square residues per 32‑block */
    const idx_t block_size = 32;     /* size of each residue block */

    /* 0‑based cycle index and position within the cycle */
    idx_t cycle = (n - 1) / per_cycle;
    idx_t offset = (n - 1) % per_cycle;

    /* Compute the block start in the result type */
    seq_t block_start = (seq_t)block_size * (seq_t)cycle;

    /* Add the appropriate residue */
    return block_start + (seq_t)nonres[offset];
}
