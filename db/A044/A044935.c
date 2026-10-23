/*
 * A044935  Number of runs of even length in the base 4 representation of n.
 * https://oeis.org/A044935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A043556(n) - A044944(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but handle 0 gracefully */

    idx_t temp = n;
    seq_t count  = 0;
    int    runlen = 0;
    int    prev_digit = -1;        /* sentinel for "no previous digit" */

    while (temp > 0) {
        int digit = temp % 4;      /* base‑4 digit (0..3) */
        temp /= 4;

        if (digit == prev_digit) {
            runlen++;
        } else {
            if (prev_digit != -1 && (runlen % 2) == 0)
                count++;
            runlen = 1;
            prev_digit = digit;
        }
    }

    /* process the final run */
    if (prev_digit != -1 && (runlen % 2) == 0)
        count++;

    return count;
}
