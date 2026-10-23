/*
 * A047587  Numbers that are congruent to {0, 2, 3, 5, 6, 7} mod 8.
 * https://oeis.org/A047587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*((n-1)/6) + offsets[(n-1)%6] where offsets =
 *            {0,2,3,5,6,7}
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial out‑of‑range case (sequence is 1‑indexed). */
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t block = (n - 1) / 6;          /* number of whole 6‑term blocks before the nth term */
    idx_t rem   = (n - 1) % 6;          /* position (0..5) within the current block */

    /* Offsets within one block of 8 integers that are NOT congruent to 1 or 4 mod 8. */
    static const unsigned char offsets[6] = {0, 2, 3, 5, 6, 7};

    /* Compute the nth value in the generic integer type seq_t. */
    seq_t value = (seq_t)8 * (seq_t)block + (seq_t)offsets[rem];
    return value;
}
