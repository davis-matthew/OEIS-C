/*
 * A221469  Number of increasing peak values in the Collatz (3x+1) iteration
 *          of n.
 * https://oeis.org/A221469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: number of increasing peak values in the Collatz iteration of n */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* offset starts at 1, just in case */
    seq_t x = (seq_t)n;
    seq_t peak = x;
    seq_t count = 0;
    while (x > (seq_t)1) {
        if ((x % (seq_t)2) == 0) {
            x /= (seq_t)2;
        } else {
            x = x * (seq_t)3 + (seq_t)1;
        }
        if (x > peak) {
            peak = x;
            ++count;
        }
    }
    return count;
}
