/*
 * A047411  Numbers that are congruent to {1, 2, 4, 6} mod 8.
 * https://oeis.org/A047411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n‑1)/4) + r where r = {1, 2, 4, 6}[(n‑1) mod 4]
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A047411: numbers congruent to {1,2,4,6} mod 8. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1; behaviour for 0 is unused */
        return 0;

    /* Determine the block number (0‑based) and the remainder index within the block. */
    idx_t q = (n - 1) / 4;                   /* block number */
    int   r_index = (int)((n - 1) % 4);      /* 0..3 */

    /* Remainder values for the four positions. */
    static const seq_t rem[4] = {1, 2, 4, 6};

    seq_t result = (seq_t)q * (seq_t)8 + rem[r_index];

    return result;
}
