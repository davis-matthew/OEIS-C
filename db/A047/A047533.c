/*
 * A047533  Numbers that are congruent to {1, 2, 3, 7} mod 8.
 * https://oeis.org/A047533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * floor((n-1)/4) + [1, 2, 3, 7][(n-1) mod 4]
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A047533: numbers congruent to {1,2,3,7} mod 8, offset 1 */

seq_t a(idx_t n)
{
        /* n is guaranteed to be >= 1 */
        idx_t block = (n-1) / 4;              /* number of complete 8‑blocks before the target */
        int    r     = (int)((n-1) % 4);      /* position within the 4 accepted residues */

        static const seq_t offsets[4] = { (seq_t)1,
                                          (seq_t)2,
                                          (seq_t)3,
                                          (seq_t)7 };

        return (seq_t)(block * (seq_t)8) + offsets[r];
}
