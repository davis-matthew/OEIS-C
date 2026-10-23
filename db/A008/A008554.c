/*
 * A008554  Numbers that do not contain the letter `y'.
 * https://oeis.org/A008554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = floor((n-1)/20)*100 + (n-1)%20
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 1, ..., a(20) = 19, a(21) = 100, a(22) = 101, ... */

seq_t a(idx_t n)
{
    /* n is 1-based index, guaranteed n >= 1 */
    idx_t m = n - 1;                       /* zero-based offset */
    seq_t block = ((seq_t)(m / 20)) * 100; /* complete hundreds */
    seq_t offset = (seq_t)(m % 20);        /* within the 100-block */
    return block + offset;
}
