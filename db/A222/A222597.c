/*
 * A222597  Number of terms of the Collatz (3x+1) trajectory of n that are
 *          greater than n.
 * https://oeis.org/A222597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of terms of the Collatz (3x+1) trajectory of n that are >
 *            n
 * Range:     u8 n <= 26, u16 n <= 5678, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t cur = (seq_t)n;   /* current term of the Collatz sequence */
    seq_t count = 0;        /* number of terms > n encountered */

    /* If n == 1, the sequence stops immediately; count stays 0. */
    while (cur != 1) {
        /* Next Collatz term */
        if ((cur & 1) == 0) {
            cur = cur >> 1;          /* even: divide by 2 */
        } else {
            cur = cur * 3 + 1;       /* odd: 3x + 1 */
        }

        /* Count the new term if it exceeds the original n */
        if (cur > (seq_t)n) {
            ++count;
        }
    }

    return count;
}
