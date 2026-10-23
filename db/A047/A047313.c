/*
 * A047313  Numbers that are congruent to {1, 4, 5, 6} mod 7.
 * https://oeis.org/A047313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/4) + {1,4,5,6}[(n-1) mod 4]
 * Range:     u8 n <= 145, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 0‑based index */
    const seq_t residues[4] = {1, 4, 5, 6};

    idx_t offset = n - 1;              /* (n-1) fits in idx_t */
    seq_t k      = (seq_t)offset / 4;   /* floor((n-1)/4) */
    seq_t r      = (seq_t)offset % 4;   /* (n-1) mod 4 */

    return k * (seq_t)7 + residues[(int)r];
}
