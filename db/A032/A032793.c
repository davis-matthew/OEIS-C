/*
 * A032793  Numbers that are congruent to {1, 2, 4} mod 5.
 * https://oeis.org/A032793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((5*n-2)/3)
 * Range:     u8 n <= 153, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A032793: numbers congruent to {1,2,4} mod 5. */
seq_t a(idx_t n)
{
    /* The offset is 1, so a(1) = 1.  For n == 0 a(0) is undefined, return 0. */
    if (n == 0)
        return 0;

    /* Integer arithmetic is sufficient: 5*n-2 is always positive for n≥1,
       and division by 3 truncates toward zero which equals the floor for positive values. */
    return ((((seq_t)n) * ((seq_t)5)) - ((seq_t)2)) / ((seq_t)3);
}
