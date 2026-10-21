/*
 * A083120  Numbers that are congruent to {0, 2, 4, 5, 7, 9, 10} mod 12.
 * https://oeis.org/A083120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*⌊(n-1)/7⌋ + offset[(n-1) mod 7] with offset = {0, 2,
 *            4, 5, 7, 9, 10}. This gives the nth integer that is congruent to
 *            0,2,4,5,7,9,or10 modulo 12, for n ≥ 1
 * Range:     u8 n <= 149, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is periodic modulo 12 with period 7.  The
       offsets for the first period (n = 1..7) are:
         0, 2, 4, 5, 7, 9, 10. */
    static const unsigned char offs[7] = {0, 2, 4, 5, 7, 9, 10};

    /* Compute the number of complete periods and the position
       within the current period.  All arithmetic is in idx_t
       (the integer type of the input index). */
    idx_t period = (n - 1) / 7;          /* number of complete 7-term blocks */
    idx_t pos    = (n - 1) % 7;          /* position within the block (0..6) */

    /* Convert to seq_t as required by the function signature.
       The multiplications are performed in seq_t to avoid overflow
       on small seq_t types where the values wrap naturally. */
    return (seq_t)12 * (seq_t)period + (seq_t)offs[pos];
}
