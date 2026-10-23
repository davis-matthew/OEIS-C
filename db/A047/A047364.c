/*
 * A047364  Numbers that are congruent to {3, 4, 5} mod 7.
 * https://oeis.org/A047364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/3) + 3 + ((n-1) mod 3)
 * Range:     u8 n <= 109, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of all positive integers congruent to 3, 4 or 5 modulo 7. */
seq_t a(idx_t n)
{
    /* n >= 1 is assumed.  All arithmetic is performed in seq_t to keep values exact. */
    idx_t k = (n - 1) / 3;              /* number of full 7‑block repetitions before the term */
    int r = (int)((n - 1) % 3);         /* position within the current block: 0,1,2 */
    seq_t term = ((seq_t)k) * 7 + (seq_t)(3 + r);
    return term;
}
