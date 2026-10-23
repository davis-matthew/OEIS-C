/*
 * A047493  Numbers that are congruent to {1, 4, 5, 7} mod 8.
 * https://oeis.org/A047493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + {1,4,5,7}[ovr] where ovr = (n-1) mod 4
 *            (the sequence starts at n = 1)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 (outside the offset) we simply return 0. */
    if (n == 0) {
        return 0;
    }

    /* Sequence pattern repeats every 4 indices:
       remainder 0 → +1, 1 → +4, 2 → +5, 3 → +7. */
    static const seq_t add[4] = {(seq_t)1, (seq_t)4, (seq_t)5, (seq_t)7};

    idx_t block = (n - 1) / 4;          /* which 8‑number block */
    idx_t rem   = (n - 1) % 4;          /* position inside the block */

    return ((seq_t)block) * 8 + add[rem];
}
