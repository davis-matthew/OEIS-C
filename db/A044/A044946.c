/*
 * A044946  Runs of odd length in the base 6 representation of n.
 * https://oeis.org/A044946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of odd‑length runs in the base‑6 representation of
 *            n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Return 0 for n == 0 (outside the offset) */
    if (n == 0) return 0;

    idx_t   tmp       = n;
    int     prev_digit = -1;  /* sentinel for "no previous digit" */
    unsigned int run_len = 0;
    seq_t   count = 0;

    while (tmp > 0) {
        int digit = (int)(tmp % 6);
        tmp /= 6;

        if (digit != prev_digit) {
            if (prev_digit != -1 && (run_len & 1))   /* odd length */
                ++count;
            prev_digit = digit;
            run_len = 1;
        } else {
            ++run_len;
        }
    }

    /* account for the last run */
    if (prev_digit != -1 && (run_len & 1))
        ++count;

    return count;
}
