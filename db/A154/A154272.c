/*
 * A154272  1,0,1 followed by 0,0,0,...
 * https://oeis.org/A154272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1 if n = 1 or n = 3; 0 otherwise.
 */

seq_t a(idx_t n)
{
    /* a(1) and a(3) are the only indices for which 1/sin(π/(2n)) is an integer */
    if (n == 1 || n == 3)
        return (seq_t)1;
    return (seq_t)0;
}
