/*
 * A044937  Number of runs of even length in base-6 representation of n.
 * https://oeis.org/A044937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of runs of even length in the base‑6
 *            representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* a(0) is not defined but harmless */

    /* Count runs of equal digits in base‑6. */
    seq_t count = 0;                   /* number of even‑length runs */
    idx_t  tmp    = n;                 /* working copy of n */
    int    last_digit = -1;            /* last digit seen (-1 means none) */
    idx_t  runlen   = 0;               /* length of the current run */

    while (tmp > 0) {
        int digit = tmp % 6;           /* next base‑6 digit (least significant) */
        tmp /= 6;

        if (digit == last_digit) {
            /* extend the current run */
            runlen++;
        } else {
            /* finalize the previous run, if any */
            if (last_digit != -1 && (runlen & 1U) == 0)
                count++;

            /* start a new run */
            last_digit = digit;
            runlen = 1;
        }
    }

    /* account for the final run */
    if (last_digit != -1 && (runlen & 1U) == 0)
        count++;

    return count;
}
