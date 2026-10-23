/*
 * A047302  Numbers that are congruent to {1, 2, 3, 4, 6} mod 7.
 * https://oeis.org/A047302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047302: Numbers congruent to {1,2,3,4,6} mod 7.
   Formula: a(n) = 7 * floor((n-1)/5) + offset[(n-1) mod 5],
   where offset = {1,2,3,4,6}.  */
seq_t a(idx_t n)
{
    /* Ensure n >= 1; behaviour for n < 1 is undefined. */
    idx_t pos   = (n - 1) % 5;   /* remainder 0..4 */
    idx_t block = (n - 1) / 5;   /* floor division */

    /* Multiply by 7 using seq_t to avoid overflow on small types. */
    seq_t base = (seq_t)block * 7;

    /* Offsets are constant for all sizes of seq_t. */
    static const seq_t offs[5] = {1, 2, 3, 4, 6};

    return base + offs[pos];
}
