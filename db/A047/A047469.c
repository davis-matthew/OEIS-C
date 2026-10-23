/*
 * A047469  Numbers that are congruent to {0, 1, 2} mod 8.
 * https://oeis.org/A047469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 5*floor((n-1)/3) - 1
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  Guarding for n == 0
       protects against accidental calls but is otherwise unused. */
    if (n == 0) {
        return 0;
    }
    idx_t k = (n - 1) / 3;            /* floor((n-1)/3) */
    return ((seq_t)n) + ((seq_t)5 * (seq_t)k) - 1;
}
