/*
 * A075480  Number of iteration that first becomes smaller than the initial
 *          value if Collatz function (A006370) is iterated, starting with
 *          numbers of the form 64n + 39.
 * https://oeis.org/A075480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of terms (including the starting value) until the
 *            first term < 64*n+39 is reached in the Collatz sequence starting
 *            from 64*n+39
 * Range:     u8 none, u16 n <= 12, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* starting value 64*n+39 */
    seq_t start = (seq_t)64 * n + (seq_t)39;
    seq_t count = 1;                  /* count the starting value */
    seq_t x = start;

    while (x >= start) {
        if ((x & (seq_t)1) == 0) {
            x = x >> 1;
        } else {
            x = x * (seq_t)3 + (seq_t)1;
        }
        count++;
    }
    return count;
}
