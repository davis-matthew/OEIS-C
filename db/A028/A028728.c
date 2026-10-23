/*
 * A028728  Nonsquares mod 15.
 * https://oeis.org/A028728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A028728 – Nonsquares mod 15
 *  a(n) = 15 * floor((n-1)/9) + r,
 *  where r is the (n-1) mod 9‑th residue that is not a quadratic residue
 *  modulo 15 (i.e. r ∉ {0,1,4,6,9,10}).
 */

seq_t a(idx_t n)
{
    /* Compute the 0‑based block index and position inside the block */
    idx_t block = (n - 1) / 9;
    idx_t pos   = (n - 1) % 9;

    /* Find the pos‑th allowed residue modulo 15 */
    idx_t count = 0;
    idx_t r;
    for (r = 0; r < 15; ++r) {
        /* Skip quadratic residues modulo 15: 0,1,4,6,9,10 */
        if (r == 0 || r == 1 || r == 4 || r == 6 || r == 9 || r == 10)
            continue;
        if (count == pos)
            break;             /* found the wanted residue */
        ++count;
    }

    return (seq_t)15 * (seq_t)block + (seq_t)r;
}
