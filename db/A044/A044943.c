/*
 * A044943  Number of runs of odd length in the base 3 representation of n.
 * https://oeis.org/A044943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of runs of odd length in the base‑3 representation
 *            of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {          /* just in case; offset starts at 1 */
        return 0;
    }
    seq_t count = 0;          /* result */
    int prev = -1;            /* previous digit, -1 means none yet */
    unsigned run_len = 0;     /* length of current run */

    idx_t temp = n;
    while (temp > 0) {
        int d = (int)(temp % 3);
        if (prev == d) {
            run_len++;
        } else {
            if (prev != -1 && (run_len & 1)) {
                count++;
            }
            prev = d;
            run_len = 1;
        }
        temp /= 3;
    }

    /* account for the last run */
    if (prev != -1 && (run_len & 1)) {
        count++;
    }

    return count;
}
