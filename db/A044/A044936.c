/*
 * A044936  Number of runs of even length in base-5 representation of n.
 * https://oeis.org/A044936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of runs of even length in base-5 representation of
 *            n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The value is small, a single seq_t variable is enough */
    seq_t count = 0;
    seq_t run_len = 0;
    int prev_digit = -1; /* sentinel for "no previous digit" */

    /* Work on a copy so that the original idx_t parameter is unchanged */
    idx_t m = n;

    /* Process digits from least significant to most significant.  The
       counting of runs is independent of digit order. */
    while (m > 0) {
        int digit = m % 5;          /* current least significant digit */
        if (digit == prev_digit) {
            run_len++;              /* continued run */
        } else {
            /* Finish previous run if present */
            if (run_len > 0 && (run_len & 1) == 0) {
                count++;
            }
            run_len = 1;            /* start new run */
            prev_digit = digit;
        }
        m /= 5;
    }

    /* After the loop, final run needs to be accounted for */
    if (run_len > 0 && (run_len & 1) == 0) {
        count++;
    }

    return count;
}
